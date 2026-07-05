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
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License, version 3,
 * along with this program. If not, see <http://www.gnu.org/licenses/>
 *
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

#define MAX_NAME_LENGTH         256
#define MAX_PATH_LENGTH         256
#define MAX_IMAGE_LENGTH        256
#define MAX_HOSTNAME_LENGTH     256
#define CONFIG_ARG_MAX_BYTES    256
#define MAX_WSPATH_LENGTH       512
#define MAX_BUFFER_LENGTH       1024
#define MAX_MESSAGE_SIZE        1024 * 10
//#define BPTS                    1

typedef struct {
    char name[MAX_NAME_LENGTH];
    int id;
    int defaultPriority;
    char image[MAX_IMAGE_LENGTH];
} Application;

Application *applications = NULL;
int application_count = 0;

struct
{
    char url[MAX_PATH_LENGTH];
    char token[MAX_NAME_LENGTH];
} configs;

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

bool parseConf(const char * fileName)
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
void send_message_to_pts(const char *pts_name, const char *message) {
    int pts_fd = open(pts_name, O_WRONLY);
    if (pts_fd == -1) {
        perror("open");
        return;
    }

    // Write the message directly to the PTS device
    if (write(pts_fd, message, strlen(message)) == -1) {
        perror("write");
    }

    // Close the file descriptor
    close(pts_fd);
}

void broadcast_message_to_all_pts(const char *message) {
    DIR *dir;
    struct dirent *entry;

    dir = opendir("/dev/pts");
    if (dir == NULL) {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        // Skip "." and ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char pts_name[MAX_PATH_LENGTH];
        snprintf(pts_name, sizeof(pts_name), "/dev/pts/%s", entry->d_name);

        send_message_to_pts(pts_name, message);
    }

    closedir(dir);
}
#endif

void send_notification(const char *in) {
    //{"id":389,"appid":2,"message":"Ehehe","title":"test","priority":5,"date":"2024-08-01T11:46:50.622413063+02:00"}
//printf("%s\n", in);
    cJSON *in_json = cJSON_Parse(in);
    if (in_json == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        cJSON_Delete(in_json);
        return;
    }

    cJSON *title = cJSON_GetObjectItemCaseSensitive(in_json, "title");
    if (cJSON_IsString(title) && (title->valuestring != NULL))
    {
        char body[MAX_MESSAGE_SIZE] = "No message";
        char appName[MAX_NAME_LENGTH] = "Gotify";
        char imageName[FILENAME_MAX] = "";
        int priority = 0;
        int i = 0;
        cJSON *appid = cJSON_GetObjectItemCaseSensitive(in_json, "appid");
        while (cJSON_IsNumber(appid) && (i < application_count)) {
            if (appid->valueint == applications[i].id) {
                priority = applications[i].defaultPriority;
                copy_string(appName, sizeof(appName), applications[i].name);
                copy_string(imageName, sizeof(imageName), applications[i].image);
                break;
            }
            i++;
        }
        cJSON *message = cJSON_GetObjectItemCaseSensitive(in_json, "message");
	if (cJSON_IsString(message) && (message->valuestring != NULL))
        {
            snprintf(body, MAX_MESSAGE_SIZE, "%s", message->valuestring);
        } else {
            snprintf(body, MAX_MESSAGE_SIZE, "No message");
        }
        cJSON *json_priority = cJSON_GetObjectItemCaseSensitive(in_json, "priority");
        if (cJSON_IsNumber(json_priority))
        {
            priority = json_priority->valueint;
        }
        printf("%s: %s\n", appName, title->valuestring);

#ifdef _WIN32
        MessageBox(NULL, body, title->valuestring, MB_OK | MB_ICONINFORMATION);
#endif
#if LIBNOTIFY
        notify_init("Gotify Notification");
        NotifyNotification *n = notify_notification_new(title->valuestring, body, imageName);
        if (priority < 5) {
            notify_notification_set_urgency(n, NOTIFY_URGENCY_LOW);
        } else if (priority == 5) {
             notify_notification_set_urgency(n, NOTIFY_URGENCY_NORMAL);
        } else if (priority > 5) {
             notify_notification_set_urgency(n, NOTIFY_URGENCY_CRITICAL);
        }
        notify_notification_show(n, NULL);
        g_object_unref(G_OBJECT(n));
        notify_uninit();
#endif
#ifdef BPTS
        char wall[MAX_MESSAGE_SIZE];
        snprintf(wall, sizeof(wall), "Gotify: %s: %s\n  %.9000s\n", appName, title->valuestring, body);
        broadcast_message_to_all_pts(wall);
#endif
#if !LIBNOTIFY
        (void)priority;
#endif
    }

    cJSON_Delete(in_json);
}

void get_applications_data(char *response, char *gotify_url, char *gotify_token) {
    // Parse JSON response
#if !LIBCURL
    (void)gotify_token;
#endif
    Application *new_applications = NULL;
    int new_application_count = 0;
    cJSON *json = cJSON_Parse(response);
    if (json == NULL) {
        fprintf(stderr, "Failed to parse JSON\n");
        return;
    }

    if (!cJSON_IsArray(json)) {
        fprintf(stderr, "Applications response is not a JSON array\n");
        cJSON_Delete(json);
        return;
    }

    cJSON *app;
    cJSON_ArrayForEach(app, json) {
        cJSON *name = cJSON_GetObjectItemCaseSensitive(app, "name");
        cJSON *id = cJSON_GetObjectItemCaseSensitive(app, "id");
        cJSON *defaultPriority = cJSON_GetObjectItemCaseSensitive(app, "defaultPriority");
        cJSON *image = cJSON_GetObjectItemCaseSensitive(app, "image");
        Application *resized;
        char image_path[FILENAME_MAX] = "";

        if (!cJSON_IsString(name) || name->valuestring == NULL ||
            !cJSON_IsNumber(id) || !cJSON_IsNumber(defaultPriority) ||
            !cJSON_IsString(image) || image->valuestring == NULL) {
            fprintf(stderr, "Skipping invalid application entry\n");
            continue;
        }

        resized = realloc(new_applications, (size_t)(new_application_count + 1) * sizeof(Application));
        if (resized == NULL) {
            fprintf(stderr, "Out of memory while reallocating applications\n");
            break;
        }

        new_applications = resized;
        memset(&new_applications[new_application_count], 0, sizeof(Application));
        copy_string(new_applications[new_application_count].name,
                    sizeof(new_applications[new_application_count].name),
                    name->valuestring);
        new_applications[new_application_count].id = id->valueint;
        new_applications[new_application_count].defaultPriority = defaultPriority->valueint;

#if LIBCURL
        char image_url[FILENAME_MAX];
        snprintf(image_url, sizeof(image_url), "%s%s", gotify_url, image->valuestring);
        if (get_image(image_url, gotify_token, image_path) != 0) {
            image_path[0] = '\0';
        }
#else
        snprintf(image_path, sizeof(image_path), "%s%s", gotify_url, image->valuestring);
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

#if LIBCURL
#if 0
struct string {
    char *ptr;
    size_t len;
};

void init_string(struct string *s) {
    s->len = 0;
    s->ptr = malloc(s->len + 1);
    if (s->ptr == NULL) {
        fprintf(stderr, "malloc() failed\n");
        exit(EXIT_FAILURE);
    }
    s->ptr[0] = '\0';
}

size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s) {
    size_t new_len = s->len + size * nmemb;
    s->ptr = realloc(s->ptr, new_len + 1);
    if (s->ptr == NULL) {
        fprintf(stderr, "realloc() failed\n");
        exit(EXIT_FAILURE);
    }
    memcpy(s->ptr + s->len, ptr, size * nmemb);
    s->ptr[new_len] = '\0';
    s->len = new_len;

    return size * nmemb;
}

void get_applications(char *gotify_url, char *gotify_token) {
    CURL *curl;
    CURLcode res;
    struct string s;

    init_string(&s);
    curl = curl_easy_init();
    if (curl) {
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        char token_header[MAX_NAME_LENGTH];
        snprintf(token_header, sizeof(token_header), "X-Gotify-Key: %s", gotify_token);
        headers = curl_slist_append(headers, token_header);
        char gotify_apps_url[512];
        snprintf(gotify_apps_url, sizeof(gotify_apps_url), "%s/application", gotify_url);
        curl_easy_setopt(curl, CURLOPT_URL, gotify_apps_url);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
        
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        } else {
            printf("%lu bytes retrieved\n", (unsigned long)s.len);
            printf("Data: %s\n", s.ptr);
            get_applications_data(s.ptr, gotify_url, gotify_token);
        }

        free(s.ptr);
        curl_easy_cleanup(curl);
    }
}
#endif
#endif

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
static bool websocket_done = false;
// Callback for HTTP GET request
static int http_callback(struct lws *wsi, enum lws_callback_reasons reason,
                         void *user, void *in, size_t len) {
    (void)user;

    switch (reason) {
        case LWS_CALLBACK_CLIENT_APPEND_HANDSHAKE_HEADER:
        {
            unsigned char **p = (unsigned char **)in, *end = (*p) + len;
            const char *header_name = "X-Gotify-Key: ";
            size_t token_len = strlen(configs.token);

            if (lws_add_http_header_by_name(wsi, (unsigned char *)header_name, (unsigned char *)configs.token, token_len, p, end)) {
                return -1;
            }
            break;
        }
	case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
            lwsl_err("CLIENT_CONNECTION_ERROR: %s\n",
			 in ? (char *)in : "(null)");
            rest_failed = true;
            rest_done = true;
            lws_cancel_service(lws_get_context(wsi));
            break;
	case LWS_CALLBACK_ESTABLISHED_CLIENT_HTTP:
        {
            char buf[128];

            lws_get_peer_simple(wsi, buf, sizeof(buf));
	    lwsl_user("Connected to %s, http response: %d\n",
			buf, (int)lws_http_client_http_response(wsi));
            break;
        }
	/* chunks of chunked content, with header removed */
	case LWS_CALLBACK_RECEIVE_CLIENT_HTTP_READ:
	    lwsl_user("RECEIVE_CLIENT_HTTP_READ: read %d\n", (int)len);

            // Append received data to rest_response
            char *resized = realloc(rest_response, rest_response_len + len + 1);
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
            return 0; /* don't passthru */
	/* uninterpreted http content */
	case LWS_CALLBACK_RECEIVE_CLIENT_HTTP:
            {
		char buffer[MAX_BUFFER_LENGTH + LWS_PRE];
		char *px = buffer + LWS_PRE;
		int lenx = sizeof(buffer) - LWS_PRE;

		if (lws_http_client_read(wsi, &px, &lenx) < 0)
		    return -1;
            }
	    return 0; /* don't passthru */
	case LWS_CALLBACK_COMPLETED_CLIENT_HTTP:
            // HTTP request completed
	    lwsl_user("LWS_CALLBACK_COMPLETED_CLIENT_HTTP: %s\n", rest_response ? rest_response : "");
            if (rest_response != NULL) {
                get_applications_data(rest_response, configs.url, configs.token);
            }
            rest_done = true;

            // After parsing, we can start the WebSocket connection
	    lws_cancel_service(lws_get_context(wsi)); /* abort poll wait */
	    break;
	case LWS_CALLBACK_CLOSED_CLIENT_HTTP:
            if (!rest_done) {
                rest_failed = true;
                rest_done = true;
            }
            lws_cancel_service(lws_get_context(wsi)); /* abort poll wait */
            break;
        default:
            break;
    }
    return 0;
}

static int callback_websockets(struct lws *wsi, enum lws_callback_reasons reason,
                             void *user, void *in, size_t len) {
    struct per_session_data *pss = (struct per_session_data *)user;
    switch (reason) {
        case LWS_CALLBACK_CLIENT_APPEND_HANDSHAKE_HEADER:
        {
            unsigned char **p = (unsigned char **)in, *end = (*p) + len;
            const char *header_name = "X-Gotify-Key: ";
            //size_t header_len = strlen(header_name);
            size_t token_len = strlen(configs.token);

            if (pss != NULL) {
                memset(pss, 0, sizeof(*pss));
                pss->token = configs.token;
            }

            if (lws_add_http_header_by_name(wsi, (unsigned char *)header_name, (unsigned char *)configs.token, token_len, p, end)) {
                return -1;
            }
            break;
        }
        case LWS_CALLBACK_CLIENT_RECEIVE:
            // Print the received message
            if (pss == NULL) {
                return -1;
            }
            lwsl_user("Received %zu bytes\n", len);
            if (!pss->buffer) {
                pss->buffer = (char *)malloc(MAX_MESSAGE_SIZE + 1);
                if (!pss->buffer) {
                    lwsl_warn("Failed to allocate buffer\n");
                    return -1;
                }
                pss->buffer_size = MAX_MESSAGE_SIZE + 1;
                pss->received_size = 0;
            }

            if (len >= pss->buffer_size || pss->received_size > pss->buffer_size - len - 1) {
                lwsl_warn("Message %zu exceeds buffer size %zu\n", pss->received_size + len, pss->buffer_size);
                free(pss->buffer);
                pss->buffer = NULL;
                pss->received_size = 0;
                return -1;
            }

            memcpy(pss->buffer + pss->received_size, in, len);
            pss->received_size += len;

            // Check if the message is complete
            if (lws_is_final_fragment(wsi)) {
                // Process the complete message
                // process_message(pss->buffer, pss->received_size);
                pss->buffer[pss->received_size] = '\0';
                send_notification(pss->buffer);
                // Free the buffer
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
            lwsl_warn("Connection error\n");
            websocket_done = true;
            if (pss != NULL) {
                pss->connected = false;
            }
            if (pss != NULL && pss->buffer) {
                free(pss->buffer);
                pss->buffer = NULL;
                pss->received_size = 0;
            }
            break;

        case LWS_CALLBACK_CLIENT_CLOSED:
            lwsl_user("Connection closed\n");
            websocket_done = true;
            if (pss != NULL) {
                pss->connected = false;
            }
            if (pss != NULL && pss->buffer) {
                free(pss->buffer);
                pss->buffer = NULL;
                pss->received_size = 0;
            }
            break;

        default:
            //lwsl_user("Unkown reason %u\n", reason);
            break;
    }
    return 0;
}

// Returns true if connection was successful and clean, false if ended due to disconnect/error
bool run_websocket_session(struct lws_context *context, 
                          struct lws_client_connect_info *connect_info) {
    websocket_done = false;
    struct lws *wsi = lws_client_connect_via_info(connect_info);
    if (!wsi) {
        fprintf(stderr, "Connection failed\n");
        return false;
    }
    // Loop as long as connected
    while (!websocket_done && lws_service(context, 100) >= 0);
    return false;
}
#endif

#if LIBWEBSOCKETS
bool parse_url(const char *url, char *hostname, size_t hostname_size, int *port, char *path, size_t path_size, int *use_ssl) {
    const char *tmp;
    size_t hostname_len;

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

    tmp = strchr(url, '/');
    if (tmp) {
        hostname_len = (size_t)(tmp - url);
        if (hostname_len >= hostname_size) {
            fprintf(stderr, "Hostname is too long\n");
            return false;
        }
        snprintf(path, path_size, "%s", tmp);
        memcpy(hostname, url, hostname_len);
        hostname[hostname_len] = '\0';
    } else {
        snprintf(path, path_size, "/");
        snprintf(hostname, hostname_size, "%s", url);
    }

    char *port_separator = strchr(hostname, ':');
    if (port_separator) {
        *port_separator = '\0';
        *port = atoi(port_separator + 1);
        if (*port <= 0 || *port > 65535) {
            fprintf(stderr, "Invalid URL port\n");
            return false;
        }
    }

    return true;
}

static void build_endpoint_path(const char *base_path, const char *endpoint, char *out, size_t out_size)
{
    size_t len = strlen(base_path);

    if (strcmp(base_path, "/") == 0) {
        snprintf(out, out_size, "/%s", endpoint);
    } else if (len > 0 && base_path[len - 1] == '/') {
        snprintf(out, out_size, "%s%s", base_path, endpoint);
    } else {
        snprintf(out, out_size, "%s/%s", base_path, endpoint);
    }
}
#endif

#ifdef _WIN32
void daemonize()
{
}
#else
void daemonize()
{
    int i;
    pid_t pid, sid;
    if(getppid()==1) return; /* already a daemon */
    pid = fork();
    if (pid < 0) exit(EXIT_FAILURE); /* fork error */
    if (pid > 0) exit(EXIT_SUCCESS); /* parent exits */
    umask(0);  /* Change the file mode mask */
    /* child (daemon) continues */
    sid = setsid(); /* obtain a new process group */
    if (sid < 0) exit(EXIT_FAILURE);
    if ((chdir("/")) < 0) exit(EXIT_FAILURE);
    i = (int)sysconf(_SC_OPEN_MAX);
    if (i < 0) {
        i = 1024;
    }
    for (; i >= 0; --i) close(i); /* close all descriptors */
    i = open("/dev/null",O_RDWR); dup(i); dup(i); /* handle standart I/O */
}
#endif

int main(int argc, const char **argv) {
    if (argc < 2) {
        if (parseConf("GotifyClientApp.conf") == false) {
            fprintf(stderr, "Usage: %s <Gotify Config File>\n", argv[0]);
            return 1;
        }
    } else if (parseConf(argv[1]) == false) {
       return 1;
    }

    if (argc < 3)
        daemonize();

#if LIBWEBSOCKETS
    char hostname[MAX_HOSTNAME_LENGTH];
    int port;
    char path[MAX_PATH_LENGTH];
    int use_ssl;
    int retry_count = 0;
    int max_delay = 60; // seconds, max backoff
    int base_delay = 1; // seconds, initial backoff

    if (!parse_url(configs.url, hostname, sizeof(hostname), &port, path, sizeof(path), &use_ssl)) {
        return 1;
    }

    char ws_path[MAX_WSPATH_LENGTH];
    char application_path[MAX_WSPATH_LENGTH];
    build_endpoint_path(path, "stream", ws_path, sizeof(ws_path));
    build_endpoint_path(path, "application", application_path, sizeof(application_path));

#if LIBCURL
#if 0
    get_applications(configs.url, (char *)configs.token);
#endif
#endif

    struct lws_context_creation_info context_info;
    struct lws_client_connect_info connect_info;
    struct lws_protocols protocols[] = {
        {
            "http",
            http_callback,
            0,
            65536,
        },
        {
            "websocket-protocol",
            callback_websockets,
            sizeof(struct per_session_data),
            65536,
        },
        { NULL, NULL, 0, 0 } /* terminator */
    };
    struct lws *wsi;

    memset(&context_info, 0, sizeof(context_info));
    lws_cmdline_option_handle_builtin(argc, argv, &context_info);
    context_info.protocols = protocols;
    if (use_ssl) {
        context_info.options |= LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT;
    }

    struct lws_context *context = lws_create_context(&context_info);
    if (!context) {
        fprintf(stderr, "lws init failed\n");
        return 1;
    }

    // Perform HTTP GET request to Gotify REST API
    struct lws_client_connect_info connect_info_http;
    memset(&connect_info_http, 0, sizeof(connect_info_http));
    connect_info_http.context = context;
    connect_info_http.address = hostname;
    connect_info_http.port = port;
    connect_info_http.path = application_path;
    connect_info_http.host = hostname;
    connect_info_http.origin = hostname;
    connect_info_http.protocol = protocols[0].name;
    if (use_ssl) {
        connect_info_http.ssl_connection = LCCSCF_USE_SSL;
    }
    connect_info_http.method = "GET";

    wsi = lws_client_connect_via_info(&connect_info_http);
    if (wsi == NULL) {
        fprintf(stderr, "Failed to perform HTTP GET request\n");
        lws_context_destroy(context);
        return -1;
    }

    // Service loop to process HTTP response
    rest_done = false;
    rest_failed = false;
    while (!rest_done && lws_service(context, 1000) >= 0);
    if (rest_failed) {
        fprintf(stderr, "Failed to fetch Gotify applications\n");
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
    connect_info.pwsi = NULL;
    if (use_ssl) {
        connect_info.ssl_connection = LCCSCF_USE_SSL;
    }

    while (1) {
        // prepare connect_info with pss, context, etc.
        bool clean_exit = run_websocket_session(context, &connect_info);
        if (clean_exit) {
            // Only happens if intentionally closed
            break;
        }
        // Exponential backoff
        int delay = base_delay << retry_count; // double each time
        if (delay > max_delay) delay = max_delay;
        lwsl_warn("WebSocket disconnected, retrying in %d seconds...\n", delay);
        sleep(delay);
        retry_count++;
    }

    lws_context_destroy(context);
#else
    fprintf(stderr, "libwebsockets support is disabled\n");
    return 1;
#endif

    return 0;
}
