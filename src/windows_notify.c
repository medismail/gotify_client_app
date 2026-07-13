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
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#ifndef ARRAYSIZE
#define ARRAYSIZE(a) (sizeof(a) / sizeof((a)[0]))
#endif

#define WM_TRAYICON (WM_USER + 1)
#define ID_TRAY_EXIT 1001
#define ID_TRAY_TEST_NOTIFICATION 1002

static NOTIFYICONDATAW nid;
static HMENU tray_menu = NULL;
static HWND tray_hwnd = NULL;
static HINSTANCE tray_instance = NULL;
static HICON tray_icon = NULL;
static bool tray_icon_owned = false;
static bool tray_added = false;
static bool quit_requested = false;
static UINT taskbar_created_message = 0;

static LRESULT CALLBACK tray_window_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

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
    if (tray_hwnd == NULL || nid.cbSize == 0) {
        return false;
    }

    if (!Shell_NotifyIconW(NIM_ADD, &nid)) {
        tray_added = false;
        return false;
    }

    tray_added = true;
    nid.uVersion = NOTIFYICON_VERSION_4;
    Shell_NotifyIconW(NIM_SETVERSION, &nid);
    return true;
}

static void destroy_tray_resources(void)
{
    if (tray_added) {
        Shell_NotifyIconW(NIM_DELETE, &nid);
        tray_added = false;
    }

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

static void execute_tray_command(UINT command)
{
    if (command == ID_TRAY_EXIT) {
        quit_requested = true;
        PostQuitMessage(0);
    } else if (command == ID_TRAY_TEST_NOTIFICATION) {
        windows_show_notification("Notification",
                                  "This is a Test Notification",
                                  5);
    }
}

static void show_context_menu(HWND hwnd, POINT pt)
{
    UINT command;

    if (tray_menu == NULL) {
        return;
    }

    SetForegroundWindow(hwnd);
    command = TrackPopupMenu(tray_menu,
                             TPM_LEFTALIGN | TPM_BOTTOMALIGN | TPM_RIGHTBUTTON |
                                 TPM_RETURNCMD | TPM_NONOTIFY,
                             pt.x,
                             pt.y,
                             0,
                             hwnd,
                             NULL);

    /* Required by the Win32 notification-area menu contract. Without this,
       later right-clicks can leave the popup owner in an inconsistent state. */
    PostMessageW(hwnd, WM_NULL, 0, 0);
    execute_tray_command(command);
}

bool windows_notifications_init(void)
{
    const wchar_t class_name[] = L"GotifyClientAppTrayWindow";
    WNDCLASSEXW wc;

    if (tray_hwnd != NULL) {
        return true;
    }

    quit_requested = false;
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
        if (tray_icon_owned && tray_icon != NULL) {
            DestroyIcon(tray_icon);
        }
        tray_icon = NULL;
        tray_icon_owned = false;
        return false;
    }

    tray_hwnd = CreateWindowExW(0,
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
    if (tray_hwnd == NULL) {
        if (tray_icon_owned && tray_icon != NULL) {
            DestroyIcon(tray_icon);
        }
        tray_icon = NULL;
        tray_icon_owned = false;
        return false;
    }

    memset(&nid, 0, sizeof(nid));
    nid.cbSize = sizeof(nid);
    nid.hWnd = tray_hwnd;
    nid.uID = 1;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
#ifdef NIF_SHOWTIP
    nid.uFlags |= NIF_SHOWTIP;
#endif
    nid.uCallbackMessage = WM_TRAYICON;
    nid.hIcon = tray_icon;
    copy_wide_truncated(nid.szTip, ARRAYSIZE(nid.szTip), L"Gotify Client App");

    if (!add_tray_icon()) {
        DestroyWindow(tray_hwnd);
        tray_hwnd = NULL;
        destroy_tray_resources();
        return false;
    }

    return true;
}

void windows_notifications_shutdown(void)
{
    HWND hwnd = tray_hwnd;

    destroy_tray_resources();

    if (hwnd != NULL) {
        tray_hwnd = NULL;
        DestroyWindow(hwnd);
    }
}

void windows_notifications_pump(void)
{
    MSG msg;

    while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            quit_requested = true;
            continue;
        }
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
}

bool windows_notifications_should_exit(void)
{
    return quit_requested;
}

void windows_show_notification(const char *title_utf8, const char *text_utf8, int priority)
{
    if (!tray_added) {
        return;
    }

    nid.uFlags |= NIF_INFO;
    nid.dwInfoFlags = priority > 5 ? NIIF_ERROR : NIIF_INFO;
    nid.uTimeout = 5000;
    copy_utf8_to_wide(nid.szInfoTitle, ARRAYSIZE(nid.szInfoTitle), title_utf8);
    copy_utf8_to_wide(nid.szInfo, ARRAYSIZE(nid.szInfo), text_utf8);

    Shell_NotifyIconW(NIM_MODIFY, &nid);
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

        case WM_TRAYICON:
        {
            UINT event = LOWORD(lparam);

            if (event == WM_CONTEXTMENU) {
                POINT pt;
                pt.x = GET_X_LPARAM((LPARAM)wparam);
                pt.y = GET_Y_LPARAM((LPARAM)wparam);
                if (pt.x == -1 && pt.y == -1) {
                    GetCursorPos(&pt);
                }
                show_context_menu(hwnd, pt);
            } else if (event == WM_RBUTTONUP) {
                POINT pt;
                GetCursorPos(&pt);
                show_context_menu(hwnd, pt);
            } else if (event == WM_LBUTTONDBLCLK || event == NIN_SELECT || event == NIN_KEYSELECT) {
                windows_show_notification("Gotify Client App",
                                          "Gotify notifications are running.",
                                          5);
            }
            break;
        }

        case WM_COMMAND:
            execute_tray_command(LOWORD(wparam));
            break;

        case WM_DESTROY:
            if (tray_hwnd == hwnd) {
                tray_hwnd = NULL;
            }
            destroy_tray_resources();
            break;

        default:
            return DefWindowProcW(hwnd, msg, wparam, lparam);
    }

    return 0;
}

#endif /* _WIN32 */
