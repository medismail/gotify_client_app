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

#define MAX_NAME_LENGTH         256
#define MAX_IMAGE_LENGTH        256
#define MAX_MESSAGE_LENGTH      1024

typedef struct {
    char name[MAX_NAME_LENGTH];
    int id;
    int defaultPriority;
    char image[MAX_IMAGE_LENGTH];
} Application;

Application *applications = NULL;
int application_count = 0;

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
        char body[MAX_MESSAGE_LENGTH];
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
            snprintf(body, MAX_MESSAGE_LENGTH, "%s", message->valuestring);
        } else {
            snprintf(body, MAX_MESSAGE_LENGTH, "No message");
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
        char token_header[256];
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
static char *rest_response = NULL;
static size_t rest_response_len = 0;
char gurl[MAX_NAME_LENGTH];
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
		char buffer[MAX_MESSAGE_LENGTH + LWS_PRE];
		char *px = buffer + LWS_PRE;
		int lenx = sizeof(buffer) - LWS_PRE;

		if (lws_http_client_read(wsi, &px, &lenx) < 0)
		    return -1;
            }
	    return 0; /* don't passthru */
	case LWS_CALLBACK_COMPLETED_CLIENT_HTTP:
            // HTTP request completed
	    lwsl_user("LWS_CALLBACK_COMPLETED_CLIENT_HTTP: %s\n", rest_response);
            get_applications_data(rest_response, gurl, (char *) user);

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
    switch (reason) {
        case LWS_CALLBACK_CLIENT_APPEND_HANDSHAKE_HEADER:
        {
            unsigned char **p = (unsigned char **)in, *end = (*p) + len;
            const char *token = (const char *)user;
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
            send_notification((char *)in);
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
        strncpy(path, tmp, 256);
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
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <Gotify URL> <Token>\n", argv[0]);
        return 1;
    }

    const char *url = argv[1];
    const char *token = argv[2];

/*    if (argc == 4)
        daemonize();*/

    char hostname[256];
    int port;
    char path[256];
    int use_ssl;
    parse_url(url, hostname, &port, path, &use_ssl);

    // Append the token to the path
    char ws_path[512];
    snprintf(ws_path, sizeof(ws_path), "%sstream", path);

#if LIBCURL
#if 0
    get_applications(url, (char *)token);
#endif
#endif

#if LIBWEBSOCKETS
    snprintf(gurl, sizeof(gurl), "%s", url);
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
    connect_info_http.userdata = (void *)token;
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

    memset(&connect_info, 0, sizeof(connect_info));
    connect_info.context = context;
    connect_info.address = hostname;
    connect_info.port = port;
    connect_info.path = ws_path;
    connect_info.host = hostname;
    connect_info.origin = hostname;
    connect_info.protocol = protocols[1].name;
    connect_info.pwsi = NULL;
    connect_info.userdata = (void *)token;
    if (use_ssl) {
        connect_info.ssl_connection = LCCSCF_USE_SSL;
    }

    wsi = lws_client_connect_via_info(&connect_info);
    if (!wsi) {
        fprintf(stderr, "Connection failed\n");
        lws_context_destroy(context);
        return 1;
    }

    while (lws_service(context, 1000) >= 0);

    lws_context_destroy(context);
#endif

    return 0;
}
