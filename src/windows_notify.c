#ifdef _WIN32

#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

#define COBJMACROS

#include "windows_notify.h"
#include "windows_resource.h"

#include <windows.h>
#include <windowsx.h>
#include <shellapi.h>
#include <initguid.h>
#include <wincodec.h>
#include <limits.h>
#include <stdint.h>
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
#ifndef NIIF_LARGE_ICON
#define NIIF_LARGE_ICON 0x00000020
#endif

#define WM_TRAYICON (WM_APP + 1)
#define WM_GOTIFY_NOTIFICATION (WM_APP + 2)
#define ID_TRAY_EXIT 1001
#define ID_TRAY_TEST_NOTIFICATION 1002
#define TRAY_SHUTDOWN_TIMEOUT_MS 5000U
#define CHANNEL_IMAGE_PATH_CHARS 1024

struct tray_notification_payload {
    wchar_t title[64];
    wchar_t text[256];
    wchar_t image_path[CHANNEL_IMAGE_PATH_CHARS];
    DWORD icon_type;
};

struct channel_icon_cache_entry {
    wchar_t *path;
    HICON icon;
    struct channel_icon_cache_entry *next;
};

/* All window, menu, WIC and icon objects are owned by tray_thread. */
static NOTIFYICONDATAW nid;
static HMENU tray_menu = NULL;
static HINSTANCE tray_instance = NULL;
static HICON tray_icon = NULL;
static bool tray_icon_owned = false;
static bool tray_added = false;
static bool tray_version_4 = false;
static UINT taskbar_created_message = 0;
static IWICImagingFactory *wic_factory = NULL;
static bool com_initialized = false;
static struct channel_icon_cache_entry *channel_icon_cache = NULL;

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

static wchar_t *duplicate_wide_string(const wchar_t *value)
{
    wchar_t *copy;
    size_t length;

    if (value == NULL) {
        return NULL;
    }

    length = wcslen(value);
    copy = (wchar_t *)malloc((length + 1) * sizeof(*copy));
    if (copy != NULL) {
        memcpy(copy, value, (length + 1) * sizeof(*copy));
    }
    return copy;
}

static bool initialize_wic(void)
{
    HRESULT hr;

    hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    if (SUCCEEDED(hr)) {
        com_initialized = true;
    } else if (hr != RPC_E_CHANGED_MODE) {
        tray_debug_message("CoInitializeEx failed; channel icons disabled");
        return false;
    }

    hr = CoCreateInstance(&CLSID_WICImagingFactory,
                          NULL,
                          CLSCTX_INPROC_SERVER,
                          &IID_IWICImagingFactory,
                          (void **)&wic_factory);
    if (FAILED(hr)) {
        wic_factory = NULL;
        tray_debug_message("WIC factory creation failed; channel icons disabled");
        return false;
    }

    return true;
}

static void clear_channel_icon_cache(void)
{
    struct channel_icon_cache_entry *entry = channel_icon_cache;

    while (entry != NULL) {
        struct channel_icon_cache_entry *next = entry->next;
        if (entry->icon != NULL) {
            DestroyIcon(entry->icon);
        }
        free(entry->path);
        free(entry);
        entry = next;
    }
    channel_icon_cache = NULL;
}

static void shutdown_wic(void)
{
    clear_channel_icon_cache();

    if (wic_factory != NULL) {
        IWICImagingFactory_Release(wic_factory);
        wic_factory = NULL;
    }

    if (com_initialized) {
        CoUninitialize();
        com_initialized = false;
    }
}

static HICON create_icon_from_wic_source(IWICBitmapSource *source,
                                         UINT width,
                                         UINT height)
{
    IWICFormatConverter *converter = NULL;
    BITMAPV5HEADER bitmap_header;
    ICONINFO icon_info;
    HBITMAP color_bitmap = NULL;
    HBITMAP mask_bitmap = NULL;
    HICON icon = NULL;
    HDC screen_dc = NULL;
    BYTE *color_bits = NULL;
    BYTE *mask_bits = NULL;
    UINT color_stride;
    UINT color_size;
    UINT mask_stride;
    size_t mask_size;
    HRESULT hr;

    if (wic_factory == NULL || source == NULL || width == 0 || height == 0 ||
        width > UINT_MAX / 4U) {
        return NULL;
    }

    color_stride = width * 4U;
    if (height > UINT_MAX / color_stride) {
        return NULL;
    }
    color_size = color_stride * height;

    hr = IWICImagingFactory_CreateFormatConverter(wic_factory, &converter);
    if (FAILED(hr)) {
        goto cleanup;
    }

    hr = IWICFormatConverter_Initialize(converter,
                                       source,
                                       &GUID_WICPixelFormat32bppPBGRA,
                                       WICBitmapDitherTypeNone,
                                       NULL,
                                       0.0,
                                       WICBitmapPaletteTypeCustom);
    if (FAILED(hr)) {
        goto cleanup;
    }

    memset(&bitmap_header, 0, sizeof(bitmap_header));
    bitmap_header.bV5Size = sizeof(bitmap_header);
    bitmap_header.bV5Width = (LONG)width;
    bitmap_header.bV5Height = -(LONG)height;
    bitmap_header.bV5Planes = 1;
    bitmap_header.bV5BitCount = 32;
    bitmap_header.bV5Compression = BI_BITFIELDS;
    bitmap_header.bV5RedMask = 0x00ff0000;
    bitmap_header.bV5GreenMask = 0x0000ff00;
    bitmap_header.bV5BlueMask = 0x000000ff;
    bitmap_header.bV5AlphaMask = 0xff000000;
    bitmap_header.bV5CSType = LCS_sRGB;

    screen_dc = GetDC(NULL);
    color_bitmap = CreateDIBSection(screen_dc,
                                    (BITMAPINFO *)&bitmap_header,
                                    DIB_RGB_COLORS,
                                    (void **)&color_bits,
                                    NULL,
                                    0);
    if (screen_dc != NULL) {
        ReleaseDC(NULL, screen_dc);
        screen_dc = NULL;
    }
    if (color_bitmap == NULL || color_bits == NULL) {
        goto cleanup;
    }

    hr = IWICFormatConverter_CopyPixels(converter,
                                        NULL,
                                        color_stride,
                                        color_size,
                                        color_bits);
    if (FAILED(hr)) {
        goto cleanup;
    }

    mask_stride = ((width + 15U) / 16U) * 2U;
    if (height > SIZE_MAX / mask_stride) {
        goto cleanup;
    }
    mask_size = (size_t)mask_stride * height;
    mask_bits = (BYTE *)calloc(1, mask_size);
    if (mask_bits == NULL) {
        goto cleanup;
    }

    mask_bitmap = CreateBitmap((int)width,
                               (int)height,
                               1,
                               1,
                               mask_bits);
    if (mask_bitmap == NULL) {
        goto cleanup;
    }

    memset(&icon_info, 0, sizeof(icon_info));
    icon_info.fIcon = TRUE;
    icon_info.hbmColor = color_bitmap;
    icon_info.hbmMask = mask_bitmap;
    icon = CreateIconIndirect(&icon_info);

cleanup:
    if (screen_dc != NULL) {
        ReleaseDC(NULL, screen_dc);
    }
    if (mask_bitmap != NULL) {
        DeleteObject(mask_bitmap);
    }
    if (color_bitmap != NULL) {
        DeleteObject(color_bitmap);
    }
    free(mask_bits);
    if (converter != NULL) {
        IWICFormatConverter_Release(converter);
    }
    return icon;
}

static HICON load_channel_icon_from_file(const wchar_t *path)
{
    IWICBitmapDecoder *decoder = NULL;
    IWICBitmapFrameDecode *frame = NULL;
    IWICBitmapScaler *scaler = NULL;
    IWICBitmapSource *source = NULL;
    HICON icon = NULL;
    UINT source_width = 0;
    UINT source_height = 0;
    UINT target_width;
    UINT target_height;
    HRESULT hr;

    if (wic_factory == NULL || path == NULL || path[0] == L'\0') {
        return NULL;
    }

    target_width = (UINT)GetSystemMetrics(SM_CXICON);
    target_height = (UINT)GetSystemMetrics(SM_CYICON);
    if (target_width == 0) {
        target_width = 32;
    }
    if (target_height == 0) {
        target_height = 32;
    }

    hr = IWICImagingFactory_CreateDecoderFromFilename(wic_factory,
                                                       path,
                                                       NULL,
                                                       GENERIC_READ,
                                                       WICDecodeMetadataCacheOnLoad,
                                                       &decoder);
    if (FAILED(hr)) {
        goto cleanup;
    }

    hr = IWICBitmapDecoder_GetFrame(decoder, 0, &frame);
    if (FAILED(hr)) {
        goto cleanup;
    }

    hr = IWICBitmapFrameDecode_GetSize(frame, &source_width, &source_height);
    if (FAILED(hr) || source_width == 0 || source_height == 0) {
        goto cleanup;
    }

    source = (IWICBitmapSource *)frame;
    if (source_width != target_width || source_height != target_height) {
        hr = IWICImagingFactory_CreateBitmapScaler(wic_factory, &scaler);
        if (FAILED(hr)) {
            goto cleanup;
        }

        hr = IWICBitmapScaler_Initialize(scaler,
                                        source,
                                        target_width,
                                        target_height,
                                        WICBitmapInterpolationModeFant);
        if (FAILED(hr)) {
            goto cleanup;
        }
        source = (IWICBitmapSource *)scaler;
    }

    icon = create_icon_from_wic_source(source, target_width, target_height);

cleanup:
    if (scaler != NULL) {
        IWICBitmapScaler_Release(scaler);
    }
    if (frame != NULL) {
        IWICBitmapFrameDecode_Release(frame);
    }
    if (decoder != NULL) {
        IWICBitmapDecoder_Release(decoder);
    }
    return icon;
}

static HICON get_channel_icon(const wchar_t *path)
{
    struct channel_icon_cache_entry *entry;
    HICON icon;

    if (path == NULL || path[0] == L'\0') {
        return NULL;
    }

    for (entry = channel_icon_cache; entry != NULL; entry = entry->next) {
        if (_wcsicmp(entry->path, path) == 0) {
            return entry->icon;
        }
    }

    icon = load_channel_icon_from_file(path);
    if (icon == NULL) {
        tray_debug_message("Unable to decode channel notification icon");
        return NULL;
    }

    entry = (struct channel_icon_cache_entry *)calloc(1, sizeof(*entry));
    if (entry == NULL) {
        DestroyIcon(icon);
        return NULL;
    }

    entry->path = duplicate_wide_string(path);
    if (entry->path == NULL) {
        DestroyIcon(icon);
        free(entry);
        return NULL;
    }

    entry->icon = icon;
    entry->next = channel_icon_cache;
    channel_icon_cache = entry;
    return icon;
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

    clear_channel_icon_cache();

    if (tray_icon_owned && tray_icon != NULL) {
        DestroyIcon(tray_icon);
    }
    tray_icon = NULL;
    tray_icon_owned = false;

    memset(&nid, 0, sizeof(nid));
}

static void show_notification_ui(const struct tray_notification_payload *payload)
{
    HICON channel_icon;

    if (!tray_added || payload == NULL) {
        return;
    }

    channel_icon = get_channel_icon(payload->image_path);
    nid.uFlags |= NIF_INFO;
    nid.uTimeout = 5000;
    copy_wide_truncated(nid.szInfoTitle, ARRAYSIZE(nid.szInfoTitle), payload->title);
    copy_wide_truncated(nid.szInfo, ARRAYSIZE(nid.szInfo), payload->text);

    if (channel_icon != NULL) {
        nid.hBalloonIcon = channel_icon;
        nid.dwInfoFlags = NIIF_USER | NIIF_LARGE_ICON;
    } else {
        nid.hBalloonIcon = NULL;
        nid.dwInfoFlags = payload->icon_type;
    }

    Shell_NotifyIconW(NIM_MODIFY, &nid);
}

static void execute_tray_command(HWND hwnd, UINT command)
{
    if (command == ID_TRAY_EXIT) {
        tray_debug_message("Exit selected");
        InterlockedExchange(&quit_requested, 1);
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

    PeekMessageW(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);
    initialize_wic();

    initialized = initialize_tray_window();
    InterlockedExchange(&tray_init_result, initialized ? 1 : -1);
    SetEvent(ready_event);

    if (!initialized) {
        shutdown_wic();
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
    shutdown_wic();
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

void windows_show_notification(const char *title_utf8,
                               const char *text_utf8,
                               const char *image_path_utf8,
                               int priority)
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
    copy_utf8_to_wide(payload->image_path,
                      ARRAYSIZE(payload->image_path),
                      image_path_utf8);
    payload->icon_type = priority > 5 ? NIIF_ERROR : NIIF_INFO;

    if (!PostMessageW(hwnd, WM_GOTIFY_NOTIFICATION, 0, (LPARAM)payload)) {
        tray_debug_message("PostMessage notification failed");
        free(payload);
    }
}

static void show_running_notification(void)
{
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
                    show_running_notification();
                }
            } else {
                if (event == WM_RBUTTONUP) {
                    POINT pt;
                    GetCursorPos(&pt);
                    show_context_menu(hwnd, pt);
                } else if (event == WM_LBUTTONDBLCLK) {
                    show_running_notification();
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
