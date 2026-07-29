/**
 * @copyright Copyright (c) 2024, Mohamed-Ismail MJRI.
 *
 * @author Mohamed-Ismail MJRI <imejri@hotmail.com>
 *
 * @license AGPL-3.0
 *
 * This code is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License, version 3,
 * as published by the Free Software Foundation.
 */

#if LIBWEBSOCKETS
#include <libwebsockets.h>
#endif

#ifdef _WIN32
#include <windows.h>
#endif
#if LIBNOTIFY
#include <libnotify/notify.h>
#endif

#include "lib/cJSON.h"
#include "main.h"
#ifdef _WIN32
#include "windows_notify.h"
#endif

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>

#if defined(BPTS) && !defined(_WIN32)
#include <dirent.h>
#endif

#ifndef _WIN32
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#endif

#define MAX_NAME_LENGTH          256
#define MAX_PATH_LENGTH          256
#define MAX_IMAGE_LENGTH         FILENAME_MAX
#define MAX_HOSTNAME_LENGTH      256
#define CONFIG_ARG_MAX_BYTES     256
#define MAX_WSPATH_LENGTH        512
#define MAX_RESOURCE_URL_LENGTH  1024
#define MAX_BUFFER_LENGTH        1024
#define MAX_MESSAGE_SIZE         (1024 * 10)

/* #define BPTS 1 */

typedef struct {
    char name[MAX_NAME_LENGTH];
    int id;
    int defaultPriority;
    char image[MAX_IMAGE_LENGTH];
} Application;

static Application *applications = NULL;
static int application_count = 0;

static struct {
    char url[MAX_PATH_LENGTH];
    char token[MAX_NAME_LENGTH];
} configs;

#if LIBCURL
static bool image_downloads_enabled = false;
#endif
#if LIBNOTIFY
static bool libnotify_initialized = false;
#endif

static char *trim_whitespace(char *value)
{
    char *end;

    while (isspace((unsigned char)*value)) {
        value++;
    }

    if (*value == '\0') {
        return value;
    }

    end = value + strlen(value) - 1;
    while (end > value && isspace((unsigned char)*end)) {
        *end = '\0';
        end--;
    }

    return value;
}

static void copy_string(char *dst, size_t dst_size, const char *src)
{
    if (dst_size == 0) {
        return;
    }

    if (src == NULL) {
        dst[0] = '\0';
        return;
    }

    snprintf(dst, dst_size, "%s", src);
}

static bool strings_equal_ignore_case(const char *left, const char *right)
{
    if (left == NULL || right == NULL) {
        return false;
    }

    while (*left != '\0' && *right != '\0') {
        if (tolower((unsigned char)*left) != tolower((unsigned char)*right)) {
            return false;
        }
        left++;
        right++;
    }

    return *left == '\0' && *right == '\0';
}

bool parseConf(const char *fileName)
{
    FILE *fp;
    char line[CONFIG_ARG_MAX_BYTES * 2];

    fp = fopen(fileName, "r");
    if (fp == NULL) {
        perror("fopen()");
        return false;
    }

    configs.url[0] = '\0';
    configs.token[0] = '\0';

    while (fgets(line, sizeof(line), fp) != NULL) {
        char *key;
        char *value;
        char *separator;

        key = trim_whitespace(line);
        if (*key == '\0' || *key == '#' || *key == ';') {
            continue;
        }

        separator = strchr(key, '=');
        if (separator == NULL) {
            fprintf(stderr, "Ignoring invalid config line: %s\n", key);
            continue;
        }

        *separator = '\0';
        value = trim_whitespace(separator + 1);
        key = trim_whitespace(key);

        if (strcmp(key, "URL") == 0) {
            copy_string(configs.url, sizeof(configs.url), value);
        } else if (strcmp(key, "Token") == 0) {
            copy_string(configs.token, sizeof(configs.token), value);
        }
    }

    fclose(fp);

    if (configs.url[0] == '\0' || configs.token[0] == '\0') {
        fprintf(stderr, "Config must contain URL and Token values\n");
        return false;
    }

    return true;
}

#ifdef BPTS
static void send_message_to_pts(const char *pts_name, const char *message)
{
    int pts_fd = open(pts_name, O_WRONLY);
    if (pts_fd == -1) {
        perror("open");
        return;
    }

    if (write(pts_fd, message, strlen(message)) == -1) {
        perror("write");
    }
    close(pts_fd);
}

static void broadcast_message_to_all_pts(const char *message)
{
    DIR *dir;
    struct dirent *entry;

    dir = opendir("/dev/pts");
    if (dir == NULL) {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        char pts_name[MAX_PATH_LENGTH];

        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        snprintf(pts_name, sizeof(pts_name), "/dev/pts/%s", entry->d_name);
        send_message_to_pts(pts_name, message);
    }

    closedir(dir);
}
#endif

static void runtime_services_init(void)
{
#if LIBCURL
    CURLcode curl_result = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (curl_result == CURLE_OK) {
        image_downloads_enabled = true;
    } else {
        fprintf(stderr,
                "Image downloads are disabled: curl_global_init failed: %s\n",
                curl_easy_strerror(curl_result));
    }
#endif

#ifdef _WIN32
    if (!windows_notifications_init()) {
        fprintf(stderr, "Windows tray notifications are disabled\n");
    }
#endif

#if LIBNOTIFY
    libnotify_initialized = notify_init("Gotify Client App") != FALSE;
    if (!libnotify_initialized) {
        fprintf(stderr, "Unable to initialize libnotify\n");
    }
#endif
}

static void runtime_services_shutdown(void)
{
#ifdef _WIN32
    windows_notifications_shutdown();
#endif

#if LIBNOTIFY
    if (libnotify_initialized) {
        notify_uninit();
        libnotify_initialized = false;
    }
#endif

#if LIBCURL
    if (image_downloads_enabled) {
        curl_global_cleanup();
        image_downloads_enabled = false;
    }
#endif
}

void send_notification(const char *in)
{
    cJSON *in_json = cJSON_Parse(in);
    cJSON *title;

    if (in_json == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        return;
    }

    title = cJSON_GetObjectItemCaseSensitive(in_json, "title");
    if (cJSON_IsString(title) && title->valuestring != NULL) {
        char body[MAX_MESSAGE_SIZE] = "No message";
        char app_name[MAX_NAME_LENGTH] = "Gotify";
        char image_name[FILENAME_MAX] = "";
        int priority = 0;
        int i = 0;
        cJSON *appid = cJSON_GetObjectItemCaseSensitive(in_json, "appid");
        cJSON *message;
        cJSON *json_priority;

        while (cJSON_IsNumber(appid) && i < application_count) {
            if (appid->valueint == applications[i].id) {
                priority = applications[i].defaultPriority;
                copy_string(app_name, sizeof(app_name), applications[i].name);
                copy_string(image_name, sizeof(image_name), applications[i].image);
                break;
            }
            i++;
        }

        message = cJSON_GetObjectItemCaseSensitive(in_json, "message");
        if (cJSON_IsString(message) && message->valuestring != NULL) {
            snprintf(body, sizeof(body), "%s", message->valuestring);
        }

        json_priority = cJSON_GetObjectItemCaseSensitive(in_json, "priority");
        if (cJSON_IsNumber(json_priority)) {
            priority = json_priority->valueint;
        }

        printf("%s: %s\n", app_name, title->valuestring);

#ifdef _WIN32
        windows_show_notification(title->valuestring,
                                  body,
                                  image_name[0] != '\0' ? image_name : NULL,
                                  priority);
#endif

#if LIBNOTIFY
        if (libnotify_initialized) {
            const char *icon = image_name[0] != '\0' ? image_name : NULL;
            NotifyNotification *notification =
                notify_notification_new(title->valuestring, body, icon);

            if (notification != NULL) {
                GError *error = NULL;

                if (priority < 5) {
                    notify_notification_set_urgency(notification, NOTIFY_URGENCY_LOW);
                } else if (priority == 5) {
                    notify_notification_set_urgency(notification, NOTIFY_URGENCY_NORMAL);
                } else {
                    notify_notification_set_urgency(notification, NOTIFY_URGENCY_CRITICAL);
                }

                if (!notify_notification_show(notification, &error)) {
                    fprintf(stderr,
                            "Unable to show desktop notification: %s\n",
                            error != NULL ? error->message : "unknown error");
                }
                if (error != NULL) {
                    g_error_free(error);
                }
                g_object_unref(G_OBJECT(notification));
            }
        }
#endif

#ifdef BPTS
        {
            char wall[MAX_MESSAGE_SIZE];
            snprintf(wall,
                     sizeof(wall),
                     "Gotify: %s: %s\n  %.9000s\n",
                     app_name,
                     title->valuestring,
                     body);
            broadcast_message_to_all_pts(wall);
        }
#endif

#if !defined(_WIN32) && !LIBNOTIFY
        (void)image_name;
        (void)priority;
#endif
    }

    cJSON_Delete(in_json);
}

static bool application_should_exit(void)
{
#ifdef _WIN32
    windows_notifications_pump();
    return windows_notifications_should_exit();
#else
    return false;
#endif
}

static void sleep_seconds(int seconds)
{
    if (seconds <= 0) {
        return;
    }

#ifdef _WIN32
    {
        DWORD remaining = (DWORD)seconds * 1000U;
        while (remaining > 0 && !application_should_exit()) {
            DWORD chunk = remaining < 100U ? remaining : 100U;
            Sleep(chunk);
            remaining -= chunk;
        }
    }
#else
    sleep((unsigned int)seconds);
#endif
}

static bool build_resource_url(const char *base_url,
                               const char *resource_path,
                               char *out,
                               size_t out_size)
{
    size_t base_length;
    bool base_has_slash;
    bool resource_has_slash;
    int written;

    if (base_url == NULL || resource_path == NULL || out == NULL || out_size == 0) {
        return false;
    }

    if (strncmp(resource_path, "http://", 7) == 0 ||
        strncmp(resource_path, "https://", 8) == 0) {
        written = snprintf(out, out_size, "%s", resource_path);
        return written >= 0 && (size_t)written < out_size;
    }

    base_length = strlen(base_url);
    base_has_slash = base_length > 0 && base_url[base_length - 1] == '/';
    resource_has_slash = resource_path[0] == '/';

    if (base_has_slash && resource_has_slash) {
        written = snprintf(out, out_size, "%s%s", base_url, resource_path + 1);
    } else if (!base_has_slash && !resource_has_slash) {
        written = snprintf(out, out_size, "%s/%s", base_url, resource_path);
    } else {
        written = snprintf(out, out_size, "%s%s", base_url, resource_path);
    }

    return written >= 0 && (size_t)written < out_size;
}

static const char *safe_image_extension(const char *image_path)
{
    const char *extension;

    if (image_path == NULL) {
        return ".img";
    }

    extension = strrchr(image_path, '.');
    if (extension == NULL || strchr(extension, '/') != NULL ||
        strchr(extension, '\\') != NULL) {
        return ".img";
    }

    if (strings_equal_ignore_case(extension, ".png") ||
        strings_equal_ignore_case(extension, ".jpg") ||
        strings_equal_ignore_case(extension, ".jpeg") ||
        strings_equal_ignore_case(extension, ".gif")) {
        return extension;
    }

    return ".img";
}

static bool build_image_cache_name(int application_id,
                                   const char *image_path,
                                   char *out,
                                   size_t out_size)
{
    int written = snprintf(out,
                           out_size,
                           "application-%d%s",
                           application_id,
                           safe_image_extension(image_path));
    return written >= 0 && (size_t)written < out_size;
}

void get_applications_data(char *response,
                           const char *gotify_url,
                           const char *gotify_token)
{
    Application *new_applications = NULL;
    int new_application_count = 0;
    cJSON *json = cJSON_Parse(response);
    cJSON *app;

    if (json == NULL) {
        fprintf(stderr, "Failed to parse applications JSON\n");
        return;
    }

    if (!cJSON_IsArray(json)) {
        fprintf(stderr, "Applications response is not a JSON array\n");
        cJSON_Delete(json);
        return;
    }

    cJSON_ArrayForEach(app, json) {
        cJSON *name = cJSON_GetObjectItemCaseSensitive(app, "name");
        cJSON *id = cJSON_GetObjectItemCaseSensitive(app, "id");
        cJSON *default_priority =
            cJSON_GetObjectItemCaseSensitive(app, "defaultPriority");
        cJSON *image = cJSON_GetObjectItemCaseSensitive(app, "image");
        Application *resized;
        char image_path[FILENAME_MAX] = "";

        if (!cJSON_IsString(name) || name->valuestring == NULL ||
            !cJSON_IsNumber(id) || !cJSON_IsNumber(default_priority) ||
            !cJSON_IsString(image) || image->valuestring == NULL) {
            fprintf(stderr, "Skipping invalid application entry\n");
            continue;
        }

        resized = (Application *)realloc(
            new_applications,
            (size_t)(new_application_count + 1) * sizeof(*new_applications));
        if (resized == NULL) {
            fprintf(stderr, "Out of memory while reallocating applications\n");
            break;
        }
        new_applications = resized;

        memset(&new_applications[new_application_count],
               0,
               sizeof(new_applications[new_application_count]));
        copy_string(new_applications[new_application_count].name,
                    sizeof(new_applications[new_application_count].name),
                    name->valuestring);
        new_applications[new_application_count].id = id->valueint;
        new_applications[new_application_count].defaultPriority =
            default_priority->valueint;

#if LIBCURL
        if (image_downloads_enabled) {
            char image_url[MAX_RESOURCE_URL_LENGTH];
            char cache_name[128];

            if (!build_resource_url(gotify_url,
                                    image->valuestring,
                                    image_url,
                                    sizeof(image_url))) {
                fprintf(stderr, "Application image URL is too long\n");
            } else if (!build_image_cache_name(id->valueint,
                                               image->valuestring,
                                               cache_name,
                                               sizeof(cache_name))) {
                fprintf(stderr, "Application image cache name is too long\n");
            } else if (get_image(image_url,
                                 gotify_token,
                                 cache_name,
                                 image_path,
                                 sizeof(image_path)) != 0) {
                image_path[0] = '\0';
            }
        }
#else
        (void)gotify_url;
        (void)gotify_token;
#endif

        copy_string(new_applications[new_application_count].image,
                    sizeof(new_applications[new_application_count].image),
                    image_path);
        new_application_count++;
    }

    if (new_application_count > 0) {
        free(applications);
        applications = new_applications;
        application_count = new_application_count;
    } else {
        free(new_applications);
    }

    cJSON_Delete(json);
}

#if LIBWEBSOCKETS
struct per_session_data {
    char *buffer;
    size_t buffer_size;
    size_t received_size;
    char *token;
    bool connected;
};

static char *rest_response = NULL;
static size_t rest_response_len = 0;
static bool rest_done = false;
static bool rest_failed = false;
static int rest_http_status = 0;
static bool websocket_done = false;

static int http_callback(struct lws *wsi,
                         enum lws_callback_reasons reason,
                         void *user,
                         void *in,
                         size_t len)
{
    (void)user;

    switch (reason) {
        case LWS_CALLBACK_CLIENT_APPEND_HANDSHAKE_HEADER:
        {
            unsigned char **position = (unsigned char **)in;
            unsigned char *end = (*position) + len;
            const char *header_name = "X-Gotify-Key: ";
            size_t token_length = strlen(configs.token);

            if (lws_add_http_header_by_name(wsi,
                                            (unsigned char *)header_name,
                                            (unsigned char *)configs.token,
                                            token_length,
                                            position,
                                            end)) {
                return -1;
            }
            break;
        }

        case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
            lwsl_err("CLIENT_CONNECTION_ERROR: %s\n",
                     in != NULL ? (char *)in : "(null)");
            rest_failed = true;
            rest_done = true;
            lws_cancel_service(lws_get_context(wsi));
            break;

        case LWS_CALLBACK_ESTABLISHED_CLIENT_HTTP:
        {
            char peer[128];
            rest_http_status = (int)lws_http_client_http_response(wsi);
            lws_get_peer_simple(wsi, peer, sizeof(peer));
            lwsl_user("Connected to %s, http response: %d\n",
                      peer,
                      rest_http_status);
            if (rest_http_status < 200 || rest_http_status >= 300) {
                rest_failed = true;
            }
            break;
        }

        case LWS_CALLBACK_RECEIVE_CLIENT_HTTP_READ:
        {
            char *resized = (char *)realloc(rest_response,
                                            rest_response_len + len + 1);
            if (resized == NULL) {
                fprintf(stderr, "Out of memory while receiving HTTP response\n");
                rest_failed = true;
                rest_done = true;
                return -1;
            }
            rest_response = resized;
            memcpy(rest_response + rest_response_len, in, len);
            rest_response_len += len;
            rest_response[rest_response_len] = '\0';
            return 0;
        }

        case LWS_CALLBACK_RECEIVE_CLIENT_HTTP:
        {
            char buffer[MAX_BUFFER_LENGTH + LWS_PRE];
            char *position = buffer + LWS_PRE;
            int available = (int)sizeof(buffer) - LWS_PRE;

            if (lws_http_client_read(wsi, &position, &available) < 0) {
                return -1;
            }
            return 0;
        }

        case LWS_CALLBACK_COMPLETED_CLIENT_HTTP:
            if (!rest_failed && rest_response != NULL) {
                get_applications_data(rest_response, configs.url, configs.token);
            }
            rest_done = true;
            lws_cancel_service(lws_get_context(wsi));
            break;

        case LWS_CALLBACK_CLOSED_CLIENT_HTTP:
            if (!rest_done) {
                rest_failed = true;
                rest_done = true;
            }
            lws_cancel_service(lws_get_context(wsi));
            break;

        default:
            break;
    }

    return 0;
}

static int callback_websockets(struct lws *wsi,
                               enum lws_callback_reasons reason,
                               void *user,
                               void *in,
                               size_t len)
{
    struct per_session_data *pss = (struct per_session_data *)user;

    switch (reason) {
        case LWS_CALLBACK_CLIENT_APPEND_HANDSHAKE_HEADER:
        {
            unsigned char **position = (unsigned char **)in;
            unsigned char *end = (*position) + len;
            const char *header_name = "X-Gotify-Key: ";
            size_t token_length = strlen(configs.token);

            if (pss != NULL) {
                memset(pss, 0, sizeof(*pss));
                pss->token = configs.token;
            }

            if (lws_add_http_header_by_name(wsi,
                                            (unsigned char *)header_name,
                                            (unsigned char *)configs.token,
                                            token_length,
                                            position,
                                            end)) {
                return -1;
            }
            break;
        }

        case LWS_CALLBACK_CLIENT_RECEIVE:
            if (pss == NULL) {
                return -1;
            }

            if (pss->buffer == NULL) {
                pss->buffer = (char *)malloc(MAX_MESSAGE_SIZE + 1);
                if (pss->buffer == NULL) {
                    lwsl_warn("Failed to allocate message buffer\n");
                    return -1;
                }
                pss->buffer_size = MAX_MESSAGE_SIZE + 1;
                pss->received_size = 0;
            }

            if (len >= pss->buffer_size ||
                pss->received_size > pss->buffer_size - len - 1) {
                lwsl_warn("Message %zu exceeds buffer size %zu\n",
                          pss->received_size + len,
                          pss->buffer_size);
                free(pss->buffer);
                pss->buffer = NULL;
                pss->received_size = 0;
                return -1;
            }

            memcpy(pss->buffer + pss->received_size, in, len);
            pss->received_size += len;

            if (lws_is_final_fragment(wsi) &&
                lws_remaining_packet_payload(wsi) == 0) {
                pss->buffer[pss->received_size] = '\0';
                send_notification(pss->buffer);
                free(pss->buffer);
                pss->buffer = NULL;
                pss->received_size = 0;
            }
            break;

        case LWS_CALLBACK_CLIENT_ESTABLISHED:
            if (pss == NULL) {
                return -1;
            }
            pss->buffer = NULL;
            pss->buffer_size = 0;
            pss->received_size = 0;
            pss->token = configs.token;
            pss->connected = true;
            lwsl_user("WebSocket connected\n");
            break;

        case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
        case LWS_CALLBACK_CLIENT_CLOSED:
            websocket_done = true;
            if (pss != NULL) {
                pss->connected = false;
                free(pss->buffer);
                pss->buffer = NULL;
                pss->received_size = 0;
            }
            break;

        default:
            break;
    }

    return 0;
}

static bool run_websocket_session(struct lws_context *context,
                                  struct lws_client_connect_info *connect_info)
{
    struct lws *wsi;

    websocket_done = false;
    wsi = lws_client_connect_via_info(connect_info);
    if (wsi == NULL) {
        fprintf(stderr, "Connection failed\n");
        return false;
    }

    while (!websocket_done && !application_should_exit() &&
           lws_service(context, 100) >= 0) {
    }

    return application_should_exit();
}

static bool parse_url(const char *url,
                      char *hostname,
                      size_t hostname_size,
                      int *port,
                      char *path,
                      size_t path_size,
                      int *use_ssl)
{
    const char *path_start;
    size_t hostname_length;
    char *port_separator;

    if (strncmp(url, "http://", 7) == 0) {
        url += 7;
        *port = 80;
        *use_ssl = 0;
    } else if (strncmp(url, "https://", 8) == 0) {
        url += 8;
        *port = 443;
        *use_ssl = 1;
    } else {
        fprintf(stderr, "Invalid URL scheme\n");
        return false;
    }

    path_start = strchr(url, '/');
    if (path_start != NULL) {
        hostname_length = (size_t)(path_start - url);
        if (hostname_length >= hostname_size) {
            fprintf(stderr, "Hostname is too long\n");
            return false;
        }
        snprintf(path, path_size, "%s", path_start);
        memcpy(hostname, url, hostname_length);
        hostname[hostname_length] = '\0';
    } else {
        snprintf(path, path_size, "/");
        snprintf(hostname, hostname_size, "%s", url);
    }

    port_separator = strchr(hostname, ':');
    if (port_separator != NULL) {
        *port_separator = '\0';
        *port = atoi(port_separator + 1);
        if (*port <= 0 || *port > 65535) {
            fprintf(stderr, "Invalid URL port\n");
            return false;
        }
    }

    return true;
}

static void build_endpoint_path(const char *base_path,
                                const char *endpoint,
                                char *out,
                                size_t out_size)
{
    size_t length = strlen(base_path);

    if (strcmp(base_path, "/") == 0) {
        snprintf(out, out_size, "/%s", endpoint);
    } else if (length > 0 && base_path[length - 1] == '/') {
        snprintf(out, out_size, "%s%s", base_path, endpoint);
    } else {
        snprintf(out, out_size, "%s/%s", base_path, endpoint);
    }
}
#endif /* LIBWEBSOCKETS */

#ifdef _WIN32
static void daemonize(void)
{
}
#else
static void daemonize(void)
{
    int descriptor;
    pid_t pid;
    pid_t sid;

    if (getppid() == 1) {
        return;
    }

    pid = fork();
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    umask(0077);
    sid = setsid();
    if (sid < 0 || chdir("/") < 0) {
        exit(EXIT_FAILURE);
    }

    descriptor = (int)sysconf(_SC_OPEN_MAX);
    if (descriptor < 0) {
        descriptor = 1024;
    }
    for (; descriptor >= 0; descriptor--) {
        close(descriptor);
    }

    descriptor = open("/dev/null", O_RDWR);
    if (descriptor >= 0) {
        dup(descriptor);
        dup(descriptor);
    }
}
#endif

int main(int argc, const char **argv)
{
    if (argc < 2) {
        if (!parseConf("GotifyClientApp.conf")) {
            fprintf(stderr, "Usage: %s <Gotify Config File>\n", argv[0]);
            return 1;
        }
    } else if (!parseConf(argv[1])) {
        return 1;
    }

    if (argc < 3) {
        daemonize();
    }

    runtime_services_init();

#if LIBWEBSOCKETS
    {
        char hostname[MAX_HOSTNAME_LENGTH];
        int port;
        char path[MAX_PATH_LENGTH];
        int use_ssl;
        int retry_count = 0;
        const int max_delay = 60;
        const int base_delay = 1;
        char ws_path[MAX_WSPATH_LENGTH];
        char application_path[MAX_WSPATH_LENGTH];
        struct lws_context_creation_info context_info;
        struct lws_client_connect_info connect_info;
        struct lws_client_connect_info connect_info_http;
        struct lws_context *context;
        struct lws *wsi;
        struct lws_protocols protocols[] = {
            { "http", http_callback, 0, 65536 },
            { "websocket-protocol", callback_websockets,
              sizeof(struct per_session_data), 65536 },
            { NULL, NULL, 0, 0 }
        };

        if (!parse_url(configs.url,
                       hostname,
                       sizeof(hostname),
                       &port,
                       path,
                       sizeof(path),
                       &use_ssl)) {
            runtime_services_shutdown();
            return 1;
        }

        build_endpoint_path(path, "stream", ws_path, sizeof(ws_path));
        build_endpoint_path(path,
                            "application",
                            application_path,
                            sizeof(application_path));

        memset(&context_info, 0, sizeof(context_info));
        lws_cmdline_option_handle_builtin(argc, argv, &context_info);
        context_info.protocols = protocols;
        if (use_ssl) {
            context_info.options |= LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT;
        }

        context = lws_create_context(&context_info);
        if (context == NULL) {
            fprintf(stderr, "lws init failed\n");
            runtime_services_shutdown();
            return 1;
        }

        memset(&connect_info_http, 0, sizeof(connect_info_http));
        connect_info_http.context = context;
        connect_info_http.address = hostname;
        connect_info_http.port = port;
        connect_info_http.path = application_path;
        connect_info_http.host = hostname;
        connect_info_http.origin = hostname;
        connect_info_http.protocol = protocols[0].name;
        connect_info_http.method = "GET";
        if (use_ssl) {
            connect_info_http.ssl_connection = LCCSCF_USE_SSL;
        }

        rest_done = false;
        rest_failed = false;
        rest_http_status = 0;
        wsi = lws_client_connect_via_info(&connect_info_http);
        if (wsi == NULL) {
            fprintf(stderr, "Failed to perform HTTP GET request\n");
            lws_context_destroy(context);
            runtime_services_shutdown();
            return 1;
        }

        while (!rest_done && !application_should_exit() &&
               lws_service(context, 1000) >= 0) {
        }
        if (rest_failed) {
            fprintf(stderr,
                    "Failed to fetch Gotify applications (HTTP %d)\n",
                    rest_http_status);
        }
        free(rest_response);
        rest_response = NULL;
        rest_response_len = 0;

        memset(&connect_info, 0, sizeof(connect_info));
        connect_info.context = context;
        connect_info.address = hostname;
        connect_info.port = port;
        connect_info.path = ws_path;
        connect_info.host = hostname;
        connect_info.origin = hostname;
        connect_info.protocol = protocols[1].name;
        if (use_ssl) {
            connect_info.ssl_connection = LCCSCF_USE_SSL;
        }

        while (!application_should_exit()) {
            bool clean_exit = run_websocket_session(context, &connect_info);
            int delay;

            if (clean_exit) {
                break;
            }

            if (retry_count >= 6) {
                delay = max_delay;
            } else {
                delay = base_delay << retry_count;
                if (delay > max_delay) {
                    delay = max_delay;
                }
            }

            lwsl_warn("WebSocket disconnected, retrying in %d seconds...\n",
                      delay);
            sleep_seconds(delay);
            if (retry_count < 30) {
                retry_count++;
            }
        }

        lws_context_destroy(context);
    }
#else
    fprintf(stderr, "libwebsockets support is disabled\n");
    runtime_services_shutdown();
    return 1;
#endif

    runtime_services_shutdown();
    free(applications);
    applications = NULL;
    application_count = 0;
    return 0;
}
