#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef LIBWEBSOCKETS
#include <libwebsockets.h>
#endif

#ifdef _WIN32
#include <windows.h>
#endif
#ifdef LIBNOTIFY
#include <libnotify/notify.h>
#endif

#include "JSON.h"

void send_notification(const char *in) {
#ifdef _WIN32
    MessageBox(NULL, in, "Notification", MB_OK | MB_ICONINFORMATION);
#endif
#ifdef LIBNOTIFY
    notify_init("Gotify Notification");
    NotifyNotification *n = notify_notification_new("Gotify Message", in, NULL);
    notify_notification_set_urgency(n, NOTIFY_URGENCY_NORMAL);
    notify_notification_show(n, NULL);
    g_object_unref(G_OBJECT(n));
    notify_uninit();
#endif
}

#ifdef LIBWEBSOCKETS
static int callback_function(struct lws *wsi, enum lws_callback_reasons reason,
                             void *user, void *in, size_t len) {
    switch (reason) {
        case LWS_CALLBACK_CLIENT_APPEND_HANDSHAKE_HEADER:
        {
            unsigned char **p = (unsigned char **)in, *end = (*p) + len;
            const char *token = (const char *)user;
            const char *header_name = "X-Gotify-Key: ";
            size_t header_len = strlen(header_name);
            size_t token_len = strlen(token);

            if (lws_add_http_header_by_name(wsi, (unsigned char *)header_name, (unsigned char *)token, token_len, p, end)) {
                return -1;
            }
            break;
        }
        case LWS_CALLBACK_CLIENT_RECEIVE:
            // Print the received message
            printf("Received: %s\n", (char *)in);
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

int main(int argc, char **argv) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <Gotify URL> <Token>\n", argv[0]);
        return 1;
    }

    const char *url = argv[1];
    const char *token = argv[2];

    char hostname[256];
    int port;
    char path[256];
    int use_ssl;
    parse_url(url, hostname, &port, path, &use_ssl);

    // Append the token to the path
    char ws_path[512];
    snprintf(ws_path, sizeof(ws_path), "%sstream", path);

#ifdef LIBWEBSOCKETS
    struct lws_context_creation_info context_info;
    struct lws_client_connect_info connect_info;
    struct lws_protocols protocols[] = {
        {
            "gotify-protocol",
            callback_function,
            0,
            65536,
        },
        { NULL, NULL, 0, 0 } /* terminator */
    };

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

    memset(&connect_info, 0, sizeof(connect_info));
    connect_info.context = context;
    connect_info.address = hostname;
    connect_info.port = port;
    connect_info.path = ws_path;
    connect_info.host = hostname;
    connect_info.origin = hostname;
    connect_info.protocol = protocols[0].name;
    connect_info.pwsi = NULL;
    connect_info.userdata = (void *)token;
    if (use_ssl) {
        connect_info.ssl_connection = LCCSCF_USE_SSL;
    }

    struct lws *wsi = lws_client_connect_via_info(&connect_info);
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
