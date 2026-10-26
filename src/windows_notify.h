#ifndef _GCA_WINDOWS_NOTIFY_H_
#define _GCA_WINDOWS_NOTIFY_H_

#ifdef _WIN32

#include <stdbool.h>
#include <stddef.h>

bool windows_notifications_init(void);
void windows_notifications_shutdown(void);
void windows_notifications_pump(void);
bool windows_notifications_should_exit(void);
void windows_show_notification(const char *title_utf8,
                               const char *text_utf8,
                               const char *image_path_utf8,
                               int priority);
bool windows_default_config_path(char *out, size_t out_size);
bool windows_settings_configure(const char *path_utf8,
                                const char *url_utf8,
                                const char *token_utf8,
                                bool open_dialog);
bool windows_settings_reload_requested(void);
bool windows_settings_take_reload(void);

#endif /* _WIN32 */

#endif /* _GCA_WINDOWS_NOTIFY_H_ */
