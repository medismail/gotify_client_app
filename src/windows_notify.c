#ifdef _WIN32

#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

#include "windows_notify.h"
#include "windows_resource.h"

#include <windows.h>
#include <windowsx.h>
#include <shellapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#ifndef ARRAYSIZE
#define ARRAYSIZE(a) (sizeof(a) / sizeof((a)[0]))
#endif

#ifndef NOTIFYICON_VERSION_4
#define NOTIFYICON_VERSION_4 4
#endif
#ifndef NIN_SELECT
#define NIN_SELECT (WM_USER + 0)
#endif
#ifndef NINF_KEY
#define NINF_KEY 0x1
#endif
#ifndef NIN_KEYSELECT
#define NIN_KEYSELECT (NIN_SELECT | NINF_KEY)
#endif

#define WM_TRAYICON (WM_APP + 1)
#define WM_GOTIFY_NOTIFICATION (WM_APP + 2)
#define ID_TRAY_EXIT 1001
#define ID_TRAY_TEST_NOTIFICATION 1002
#define TRAY_SHUTDOWN_TIMEOUT_MS 5000U

struct tray_notification_payload {
    wchar_t title[64];
    wchar_t text[256];
    DWORD icon_type;
};

/* All window, menu and icon objects are owned by tray_thread. */
static NOTIFYICONDATAW nid;
static HMENU tray_menu = NULL;
static HINSTANCE tray_instance = NULL;
static HICON tray_icon = NULL;
static bool tray_icon_owned = false;
static bool tray_added = false;
static bool tray_version_4 = false;
static UINT taskbar_created_message = 0;

/* Cross-thread state is accessed only through Interlocked operations. */
static PVOID volatile tray_hwnd_value = NULL;
static volatile LONG quit_requested = 0;
static volatile LONG tray_init_result = 0;
static HANDLE tray_thread = NULL;
static DWORD tray_thread_id = 0;
static bool tray_debug_enabled = false;

static LRESULT CALLBACK tray_window_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
static DWORD WINAPI tray_thread_proc(LPVOID parameter);

static HWND get_tray_hwnd(void)
{
    return (HWND)InterlockedCompareExchangePointer(&tray_hwnd_value, NULL, NULL);
}

static void set_tray_hwnd(HWND hwnd)
{
    InterlockedExchangePointer(&tray_hwnd_value, (PVOID)hwnd);
}

static void tray_debug_callback(UINT event)
{
    if (!tray_debug_enabled) {
        return;
    }

    fprintf(stderr,
            "[tray %lu ms] callback event=0x%04x version4=%d thread=%lu\n",
            (unsigned long)GetTickCount(),
            (unsigned int)event,
            tray_version_4 ? 1 : 0,
            (unsigned long)GetCurrentThreadId());
    fflush(stderr);
}

static void tray_debug_message(const char *message)
{
    if (!tray_debug_enabled) {
        return;
    }

    fprintf(stderr,
            "[tray %lu ms] %s thread=%lu\n",
            (unsigned long)GetTickCount(),
            message,
            (unsigned long)GetCurrentThreadId());
    fflush(stderr);
}

static void copy_wide_truncated(wchar_t *dest, size_t dest_count, const wchar_t *src)
{
    size_t i;

    if (dest_count == 0) {
        return;
    }

    if (src == NULL) {
        dest[0] = L'\0';
        return;
    }

    for (i = 0; i + 1 < dest_count && src[i] != L'\0'; i++) {
        dest[i] = src[i];
    }
    dest[i] = L'\0';
}

static void copy_utf8_to_wide(wchar_t *dest, size_t dest_count, const char *src)
{
    int required;
    wchar_t *wide;
    UINT codepage = CP_UTF8;
    DWORD flags = MB_ERR_INVALID_CHARS;

    if (dest_count == 0) {
        return;
    }

    dest[0] = L'\0';
    if (src == NULL) {
        return;
    }

    required = MultiByteToWideChar(codepage, flags, src, -1, NULL, 0);
    if (required <= 0) {
        codepage = CP_ACP;
        flags = 0;
        required = MultiByteToWideChar(codepage, flags, src, -1, NULL, 0);
    }

    if (required <= 0) {
        return;
    }

    wide = (wchar_t *)malloc((size_t)required * sizeof(*wide));
    if (wide == NULL) {
        return;
    }

    if (MultiByteToWideChar(codepage, flags, src, -1, wide, required) > 0) {
        copy_wide_truncated(dest, dest_count, wide);
    }

    free(wide);
}

static HICON load_gotify_icon(int width, int height)
{
    HICON icon;

    icon = (HICON)LoadImageW(tray_instance,
                             MAKEINTRESOURCEW(IDI_GOTIFY),
                             IMAGE_ICON,
                             width,
                             height,
                             LR_DEFAULTCOLOR);
    if (icon != NULL) {
        tray_icon_owned = true;
        return icon;
    }

    tray_icon_owned = false;
    return LoadIconW(NULL, IDI_APPLICATION);
}

static bool add_tray_icon(void)
{
    if (get_tray_hwnd() == NULL || nid.cbSize == 0) {
        return false;
    }

    if (!Shell_NotifyIconW(NIM_ADD, &nid)) {
        tray_added = false;
        tray_version_4 = false;
        return false;
    }

    tray_added = true;
    nid.uVersion = NOTIFYICON_VERSION_4;
    tray_version_4 = Shell_NotifyIconW(NIM_SETVERSION, &nid) != FALSE;
    return true;
}

static void destroy_tray_resources(void)
{
    if (tray_added) {
        Shell_NotifyIconW(NIM_DELETE, &nid);
        tray_added = false;
    }
    tray_version_4 = false;

    if (tray_menu != NULL) {
        DestroyMenu(tray_menu);
        tray_menu = NULL;
    }

    if (tray_icon_owned && tray_icon != NULL) {
        DestroyIcon(tray_icon);
    }
    tray_icon = NULL;
    tray_icon_owned = false;

    memset(&nid, 0, sizeof(nid));
}

static void show_notification_ui(const struct tray_notification_payload *payload)
{
    if (!tray_added || payload == NULL) {
        return;
    }

    nid.uFlags |= NIF_INFO;
    nid.dwInfoFlags = payload->icon_type;
    nid.uTimeout = 5000;
    copy_wide_truncated(nid.szInfoTitle, ARRAYSIZE(nid.szInfoTitle), payload->title);
    copy_wide_truncated(nid.szInfo, ARRAYSIZE(nid.szInfo), payload->text);
    Shell_NotifyIconW(NIM_MODIFY, &nid);
}

static void execute_tray_command(HWND hwnd, UINT command)
{
    if (command == ID_TRAY_EXIT) {
        tray_debug_message("Exit selected");
        InterlockedExchange(&quit_requested, 1);

        /* lws_service() can sleep indefinitely on current libwebsockets.
           Remove the icon cleanly, then terminate for an immediate user-requested exit. */
        DestroyWindow(hwnd);
        ExitProcess(0);
    } else if (command == ID_TRAY_TEST_NOTIFICATION) {
        struct tray_notification_payload payload;

        memset(&payload, 0, sizeof(payload));
        copy_wide_truncated(payload.title, ARRAYSIZE(payload.title), L"Notification");
        copy_wide_truncated(payload.text,
                            ARRAYSIZE(payload.text),
                            L"This is a Test Notification");
        payload.icon_type = NIIF_INFO;
        show_notification_ui(&payload);
    }
}

static void show_context_menu(HWND hwnd, POINT pt)
{
    UINT command;

    if (tray_menu == NULL) {
        return;
    }

    tray_debug_message("Opening context menu");
    SetForegroundWindow(hwnd);
    command = (UINT)TrackPopupMenu(tray_menu,
                                   TPM_LEFTALIGN | TPM_BOTTOMALIGN | TPM_RIGHTBUTTON |
                                       TPM_RETURNCMD | TPM_NONOTIFY,
                                   pt.x,
                                   pt.y,
                                   0,
                                   hwnd,
                                   NULL);

    /* Required by the Win32 notification-area menu contract. */
    PostMessageW(hwnd, WM_NULL, 0, 0);
    tray_debug_message("Context menu closed");
    execute_tray_command(hwnd, command);
}

static bool initialize_tray_window(void)
{
    const wchar_t class_name[] = L"GotifyClientAppTrayWindow";
    WNDCLASSEXW wc;
    HWND hwnd;

    tray_instance = GetModuleHandleW(NULL);
    taskbar_created_message = RegisterWindowMessageW(L"TaskbarCreated");
    tray_icon = load_gotify_icon(GetSystemMetrics(SM_CXSMICON),
                                 GetSystemMetrics(SM_CYSMICON));

    memset(&wc, 0, sizeof(wc));
    wc.cbSize = sizeof(wc);
    wc.lpfnWndProc = tray_window_proc;
    wc.hInstance = tray_instance;
    wc.hIcon = tray_icon;
    wc.hIconSm = tray_icon;
    wc.lpszClassName = class_name;

    if (!RegisterClassExW(&wc) && GetLastError() != ERROR_CLASS_ALREADY_EXISTS) {
        destroy_tray_resources();
        return false;
    }

    hwnd = CreateWindowExW(0,
                           class_name,
                           L"Gotify Client App",
                           WS_OVERLAPPEDWINDOW,
                           CW_USEDEFAULT,
                           CW_USEDEFAULT,
                           CW_USEDEFAULT,
                           CW_USEDEFAULT,
                           NULL,
                           NULL,
                           tray_instance,
                           NULL);
    if (hwnd == NULL) {
        destroy_tray_resources();
        return false;
    }

    set_tray_hwnd(hwnd);

    memset(&nid, 0, sizeof(nid));
    nid.cbSize = sizeof(nid);
    nid.hWnd = hwnd;
    nid.uID = 1;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
#ifdef NIF_SHOWTIP
    nid.uFlags |= NIF_SHOWTIP;
#endif
    nid.uCallbackMessage = WM_TRAYICON;
    nid.hIcon = tray_icon;
    copy_wide_truncated(nid.szTip, ARRAYSIZE(nid.szTip), L"Gotify Client App");

    if (!add_tray_icon()) {
        DestroyWindow(hwnd);
        return false;
    }

    return true;
}

static void discard_pending_notifications(void)
{
    MSG msg;

    while (PeekMessageW(&msg,
                        NULL,
                        WM_GOTIFY_NOTIFICATION,
                        WM_GOTIFY_NOTIFICATION,
                        PM_REMOVE)) {
        free((void *)msg.lParam);
    }
}

static DWORD WINAPI tray_thread_proc(LPVOID parameter)
{
    HANDLE ready_event = (HANDLE)parameter;
    MSG msg;
    bool initialized;

    /* Force creation of this thread's message queue before init is reported. */
    PeekMessageW(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);

    initialized = initialize_tray_window();
    InterlockedExchange(&tray_init_result, initialized ? 1 : -1);
    SetEvent(ready_event);

    if (!initialized) {
        return 1;
    }

    tray_debug_message("Tray message thread ready");
    while (GetMessageW(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    if (get_tray_hwnd() != NULL) {
        DestroyWindow(get_tray_hwnd());
    }
    discard_pending_notifications();
    tray_debug_message("Tray message thread stopped");
    return 0;
}

bool windows_notifications_init(void)
{
    HANDLE ready_event;
    DWORD wait_result;
    wchar_t debug_value[2];

    if (tray_thread != NULL) {
        return InterlockedCompareExchange(&tray_init_result, 0, 0) == 1;
    }

    tray_debug_enabled = GetEnvironmentVariableW(L"GOTIFY_TRAY_DEBUG",
                                                  debug_value,
                                                  ARRAYSIZE(debug_value)) > 0;
    InterlockedExchange(&quit_requested, 0);
    InterlockedExchange(&tray_init_result, 0);
    set_tray_hwnd(NULL);

    ready_event = CreateEventW(NULL, TRUE, FALSE, NULL);
    if (ready_event == NULL) {
        return false;
    }

    tray_thread = CreateThread(NULL,
                               0,
                               tray_thread_proc,
                               ready_event,
                               0,
                               &tray_thread_id);
    if (tray_thread == NULL) {
        CloseHandle(ready_event);
        return false;
    }

    wait_result = WaitForSingleObject(ready_event, INFINITE);
    CloseHandle(ready_event);

    if (wait_result != WAIT_OBJECT_0 ||
        InterlockedCompareExchange(&tray_init_result, 0, 0) != 1) {
        PostThreadMessageW(tray_thread_id, WM_QUIT, 0, 0);
        WaitForSingleObject(tray_thread, 1000U);
        CloseHandle(tray_thread);
        tray_thread = NULL;
        tray_thread_id = 0;
        return false;
    }

    return true;
}

void windows_notifications_shutdown(void)
{
    HWND hwnd;

    if (tray_thread == NULL) {
        return;
    }

    InterlockedExchange(&quit_requested, 1);
    hwnd = get_tray_hwnd();
    if (hwnd != NULL) {
        PostMessageW(hwnd, WM_CLOSE, 0, 0);
    } else {
        PostThreadMessageW(tray_thread_id, WM_QUIT, 0, 0);
    }

    WaitForSingleObject(tray_thread, TRAY_SHUTDOWN_TIMEOUT_MS);
    CloseHandle(tray_thread);
    tray_thread = NULL;
    tray_thread_id = 0;
    set_tray_hwnd(NULL);
}

void windows_notifications_pump(void)
{
    /* The tray owns a dedicated blocking GetMessage loop. */
}

bool windows_notifications_should_exit(void)
{
    return InterlockedCompareExchange(&quit_requested, 0, 0) != 0;
}

void windows_show_notification(const char *title_utf8, const char *text_utf8, int priority)
{
    struct tray_notification_payload *payload;
    HWND hwnd = get_tray_hwnd();

    if (hwnd == NULL) {
        return;
    }

    payload = (struct tray_notification_payload *)calloc(1, sizeof(*payload));
    if (payload == NULL) {
        return;
    }

    copy_utf8_to_wide(payload->title, ARRAYSIZE(payload->title), title_utf8);
    copy_utf8_to_wide(payload->text, ARRAYSIZE(payload->text), text_utf8);
    payload->icon_type = priority > 5 ? NIIF_ERROR : NIIF_INFO;

    if (!PostMessageW(hwnd, WM_GOTIFY_NOTIFICATION, 0, (LPARAM)payload)) {
        tray_debug_message("PostMessage notification failed");
        free(payload);
    }
}

static LRESULT CALLBACK tray_window_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    if (taskbar_created_message != 0 && msg == taskbar_created_message) {
        tray_added = false;
        add_tray_icon();
        return 0;
    }

    switch (msg) {
        case WM_CREATE:
            tray_menu = CreatePopupMenu();
            if (tray_menu != NULL) {
                AppendMenuW(tray_menu, MF_STRING, ID_TRAY_TEST_NOTIFICATION, L"Test Notification");
                AppendMenuW(tray_menu, MF_SEPARATOR, 0, NULL);
                AppendMenuW(tray_menu, MF_STRING, ID_TRAY_EXIT, L"Exit");
            }
            break;

        case WM_GOTIFY_NOTIFICATION:
        {
            struct tray_notification_payload *payload =
                (struct tray_notification_payload *)lparam;
            show_notification_ui(payload);
            free(payload);
            break;
        }

        case WM_TRAYICON:
        {
            UINT event = LOWORD(lparam);

            tray_debug_callback(event);
            if (tray_version_4) {
                if (event == WM_CONTEXTMENU) {
                    POINT pt;
                    pt.x = GET_X_LPARAM((LPARAM)wparam);
                    pt.y = GET_Y_LPARAM((LPARAM)wparam);
                    if (pt.x == -1 && pt.y == -1) {
                        GetCursorPos(&pt);
                    }
                    show_context_menu(hwnd, pt);
                } else if (event == NIN_SELECT || event == NIN_KEYSELECT) {
                    struct tray_notification_payload payload;
                    memset(&payload, 0, sizeof(payload));
                    copy_wide_truncated(payload.title,
                                        ARRAYSIZE(payload.title),
                                        L"Gotify Client App");
                    copy_wide_truncated(payload.text,
                                        ARRAYSIZE(payload.text),
                                        L"Gotify notifications are running.");
                    payload.icon_type = NIIF_INFO;
                    show_notification_ui(&payload);
                }
            } else {
                if (event == WM_RBUTTONUP) {
                    POINT pt;
                    GetCursorPos(&pt);
                    show_context_menu(hwnd, pt);
                } else if (event == WM_LBUTTONDBLCLK) {
                    struct tray_notification_payload payload;
                    memset(&payload, 0, sizeof(payload));
                    copy_wide_truncated(payload.title,
                                        ARRAYSIZE(payload.title),
                                        L"Gotify Client App");
                    copy_wide_truncated(payload.text,
                                        ARRAYSIZE(payload.text),
                                        L"Gotify notifications are running.");
                    payload.icon_type = NIIF_INFO;
                    show_notification_ui(&payload);
                }
            }
            break;
        }

        case WM_COMMAND:
            execute_tray_command(hwnd, LOWORD(wparam));
            break;

        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;

        case WM_DESTROY:
            destroy_tray_resources();
            set_tray_hwnd(NULL);
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProcW(hwnd, msg, wparam, lparam);
    }

    return 0;
}

#endif /* _WIN32 */
