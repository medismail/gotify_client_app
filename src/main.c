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

#include <dirent.h>
#include <fcntl.h>
#include <errno.h>

#define MAX_NAME_LENGTH         256
#define MAX_PATH_LENGTH         256
#define MAX_IMAGE_LENGTH        256
#define MAX_HOSTNAME_LENGTH     256
#define CONFIG_ARG_MAX_BYTES    256
#define MAX_WSPATH_LENGTH       512
#define MAX_BUFFER_LENGTH       1024
#define MAX_MESSAGE_SIZE        1024 * 10

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

typedef struct config_option config_option;
typedef config_option* config_option_t;

struct config_option {
    config_option_t prev;
    char key[CONFIG_ARG_MAX_BYTES];
    char value[CONFIG_ARG_MAX_BYTES];
};

config_option_t read_config_file(const char* path) {
    FILE* fp;

    if ((fp = fopen(path, "r+")) == NULL) {
        perror("fopen()");
        return NULL;
    }

    config_option_t last_co_addr = NULL;

    while (1) {
        config_option_t co = NULL;
        if ((co = calloc(1, sizeof(config_option))) == NULL)
            continue;
        memset(co, 0, sizeof(config_option));
        co->prev = last_co_addr;

        //TODO
        //Add fgets and delete space then check
        if (fscanf(fp, "%s = %s", &co->key[0], &co->value[0]) != 2) {
            if (feof(fp)) {
                break;
            }
            if (co->key[0] == '#') {
                while (fgetc(fp) != '\n') {
                    // Do nothing (to move the cursor to the end of the line).
                }
                free(co);
                continue;
            }
            perror("fscanf()");
            free(co);
            continue;
        }
        //logDebug("Key: %s, Value: %s", co->key, co->value);
        last_co_addr = co;
    }
    fclose(fp);
    return last_co_addr;
}

bool parseConf(const char * fileName)
{
    bool ret = false;
    config_option_t configOption;
    configOption = read_config_file(fileName);
    while (configOption) {
        if (strncmp(configOption->key, "URL", 3) == 0) {
            strncpy(configs.url, configOption->value, MAX_PATH_LENGTH);
            configs.url[MAX_PATH_LENGTH-1] = '\0';
        } else if (strncmp(configOption->key, "Token", 5) == 0) {
            strncpy(configs.token, configOption->value, MAX_NAME_LENGTH);
            configs.token[MAX_NAME_LENGTH-1] = '\0';
        }
        config_option_t co = configOption;
        configOption = configOption->prev;
        free(co);
    }
    ret = true;
    return ret;
}

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

void send_notification(const char *in) {
    //{"id":389,"appid":2,"message":"Ehehe","title":"test","priority":5,"date":"2024-08-01T11:46:50.622413063+02:00"}
printf("%s\n", in);
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
        char body[MAX_MESSAGE_SIZE];
        char appName[MAX_NAME_LENGTH];
        char imageName[FILENAME_MAX];
        int priority = 0;
        int i = 0;
        cJSON *appid = cJSON_GetObjectItemCaseSensitive(in_json, "appid");
        while ((appid) && (i < application_count)) {
            if (appid->valueint == applications[i].id) {
                priority = applications[i].defaultPriority;
                strncpy(appName, applications[i].name, MAX_NAME_LENGTH);
                appName[strlen(applications[i].name)] = '\0';
                strncpy(imageName, applications[i].image, FILENAME_MAX);
                imageName[strlen(applications[i].image)] = '\0';
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
        char wall[MAX_MESSAGE_SIZE];
        snprintf(wall, MAX_MESSAGE_SIZE, "Gotify: %s: %s\n  %s\n", appName, title->valuestring, body);
        broadcast_message_to_all_pts(wall);
    }

    cJSON_Delete(in_json);
}

void get_applications_data(char *response, char *gotify_url, char *gotify_token) {
    // Parse JSON response
    cJSON *json = cJSON_Parse(response);
    if (json == NULL) {
        fprintf(stderr, "Failed to parse JSON\n");
    } else {
        cJSON *app;
        cJSON_ArrayForEach(app, json) {
           applications = realloc(applications, (application_count + 1) * sizeof(Application));
           if (!applications) {
               fprintf(stderr, "Out of memory while reallocating applications\n");
               break;
            }
            cJSON *name = cJSON_GetObjectItem(app, "name");
            cJSON *id = cJSON_GetObjectItem(app, "id");
            cJSON *defaultPriority = cJSON_GetObjectItem(app, "defaultPriority");
            cJSON *image = cJSON_GetObjectItem(app, "image");
            strncpy(applications[application_count].name, name->valuestring, sizeof(applications[application_count].name) - 1);
            applications[application_count].id = id->valueint;
            applications[application_count].defaultPriority = defaultPriority->valueint;
            char image_path[FILENAME_MAX];
#if LIBCURL
            char image_url[FILENAME_MAX];
            snprintf(image_url, sizeof(image_url), "%s%s", gotify_url, image->valuestring);
            get_image(image_url, gotify_token, image_path);
#else
            snprintf(image_path, sizeof(image_path), "%s%s", gotify_url, image->valuestring);
#endif
            strncpy(applications[application_count].image, image_path, sizeof(applications[application_count].image) - 1);
            application_count++;
        }
        cJSON_Delete(json);
    }
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
};
static char *rest_response = NULL;
static size_t rest_response_len = 0;
// Callback for HTTP GET request
static int http_callback(struct lws *wsi, enum lws_callback_reasons reason,
                         void *user, void *in, size_t len) {
    switch (reason) {
        case LWS_CALLBACK_CLIENT_APPEND_HANDSHAKE_HEADER:
        {
            unsigned char **p = (unsigned char **)in, *end = (*p) + len;
            const char *token = (const char *)user;
            const char *header_name = "X-Gotify-Key: ";
            size_t token_len = strlen(token);

            if (lws_add_http_header_by_name(wsi, (unsigned char *)header_name, (unsigned char *)token, token_len, p, end)) {
                return -1;
            }
            break;
        }
	case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
            lwsl_err("CLIENT_CONNECTION_ERROR: %s\n",
			 in ? (char *)in : "(null)");
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
            rest_response = realloc(rest_response, rest_response_len + len + 1);
            if (!rest_response) {
                fprintf(stderr, "Out of memory while receiving HTTP response\n");
                return -1;
            }
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
	    lwsl_user("LWS_CALLBACK_COMPLETED_CLIENT_HTTP: %s\n", rest_response);
            get_applications_data(rest_response, configs.url, (char *) user);

            // After parsing, we can start the WebSocket connection
	    lws_cancel_service(lws_get_context(wsi)); /* abort poll wait */
	    break;
	case LWS_CALLBACK_CLOSED_CLIENT_HTTP:
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
            const char *token = (const char *)pss->token;
            const char *header_name = "X-Gotify-Key: ";
            //size_t header_len = strlen(header_name);
            size_t token_len = strlen(token);

            if (lws_add_http_header_by_name(wsi, (unsigned char *)header_name, (unsigned char *)token, token_len, p, end)) {
                return -1;
            }
            break;
        }
        case LWS_CALLBACK_CLIENT_RECEIVE:
            // Print the received message
            lwsl_user("Received: %s\n", (char *)in);
            if (!pss->buffer) {
                pss->buffer = (char *)malloc(MAX_MESSAGE_SIZE);
                if (!pss->buffer) {
                    lwsl_warn("Failed to allocate buffer\n");
                    return -1;
                }
                pss->buffer_size = MAX_MESSAGE_SIZE;
                pss->received_size = 0;
            }

            if (pss->received_size + len > pss->buffer_size) {
                lwsl_warn("Message %d exceeds buffer size %d\n", pss->received_size + len, pss->buffer_size);
                free(pss->buffer);
                pss->buffer = NULL;
                return -1;
            }

            memcpy(pss->buffer + pss->received_size, in, len);
            pss->received_size += len;

            // Check if the message is complete
            if (lws_is_final_fragment(wsi)) {
                // Process the complete message
                // process_message(pss->buffer, pss->received_size);
                send_notification(pss->buffer);
                // Free the buffer
                free(pss->buffer);
                pss->buffer = NULL;
                pss->received_size = 0;
            }

            break;

        case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
            lwsl_warn("Connection error\n");
            if (pss->buffer) {
                free(pss->buffer);
                pss->buffer = NULL;
            }
            break;

        case LWS_CALLBACK_CLOSED:
            lwsl_user("Connection closed\n");
            if (pss->buffer) {
                free(pss->buffer);
                pss->buffer = NULL;
            }
            break;

        default:
            break;
    }
    return 0;
}
#endif

void parse_url(const char *url, char *hostname, int *port, char *path, int *use_ssl) {
    char *tmp;
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
        exit(1);
    }

    tmp = strchr(url, '/');
    if (tmp) {
        strncpy(path, tmp, MAX_PATH_LENGTH);
        strncpy(hostname, url, tmp - url);
        hostname[tmp - url] = '\0';
    } else {
        strcpy(path, "/");
        strcpy(hostname, url);
    }

    tmp = strchr(hostname, ':');
    if (tmp) {
        *tmp = '\0';
        *port = atoi(tmp + 1);
    }
}

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
    for (i = getdtablesize(); i >= 0; --i) close(i); /* close all descriptors */
    i = open("/dev/null",O_RDWR); dup(i); dup(i); /* handle standart I/O */
}
#endif

int main(int argc, const char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <Gotify Config File>\n", argv[0]);
        return 1;
    }

    parseConf(argv[1]);

    if (argc < 3)
        daemonize();

    char hostname[MAX_HOSTNAME_LENGTH];
    int port;
    char path[MAX_PATH_LENGTH];
    int use_ssl;
    parse_url(configs.url, hostname, &port, path, &use_ssl);

    // Append the token to the path
    char ws_path[MAX_WSPATH_LENGTH];
    snprintf(ws_path, sizeof(ws_path), "%sstream", path);

#if LIBCURL
#if 0
    get_applications(configs.url, (char *)configs.token);
#endif
#endif

#if LIBWEBSOCKETS
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
            0,
            65536,
        },
        { NULL, NULL, 0, 0 } /* terminator */
    };
    struct lws *wsi;

    lws_cmdline_option_handle_builtin(argc, argv, &context_info);
    memset(&context_info, 0, sizeof(context_info));
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
    connect_info_http.path = "/application";
    connect_info_http.host = hostname;
    connect_info_http.origin = hostname;
    connect_info_http.protocol = protocols[0].name;
    connect_info_http.userdata = (void *)configs.token;
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
    while (lws_service(context, 1000) >= 0 && rest_response == NULL);
    free(rest_response);
    rest_response = NULL;
    rest_response_len = 0;

    struct per_session_data *pss = malloc(sizeof(struct per_session_data));
    pss->token = configs.token;
    pss->buffer = NULL;
    memset(&connect_info, 0, sizeof(connect_info));
    connect_info.context = context;
    connect_info.address = hostname;
    connect_info.port = port;
    connect_info.path = ws_path;
    connect_info.host = hostname;
    connect_info.origin = hostname;
    connect_info.protocol = protocols[1].name;
    connect_info.pwsi = NULL;
    connect_info.userdata = (void *)pss;
    if (use_ssl) {
        connect_info.ssl_connection = LCCSCF_USE_SSL;
    }

    wsi = lws_client_connect_via_info(&connect_info);
    if (!wsi) {
        fprintf(stderr, "Connection failed\n");
        free(pss);
        lws_context_destroy(context);
        return 1;
    }

    while (lws_service(context, 1000) >= 0);

    free(pss);
    lws_context_destroy(context);
#endif

    return 0;
}
