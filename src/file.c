/**
 * @copyright Copyright (c) 2024, Mohamed-Ismail MJRI.
 *
 * @author Mohamed-Ismail MJRI <imejri@hotmail.com>
 *
 * @license AGPL-3.0
 */

#include "main.h"

#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <sys/stat.h>

#ifdef _WIN32
#include <direct.h>
#include <wchar.h>
#define WAPP_FOLDER L"GotifyClientApp"
#else
#include <unistd.h>
#define APP_FOLDER "GotifyClientApp"
#endif

static bool copy_text(char *dest, size_t dest_size, const char *src)
{
    int written;

    if (dest == NULL || dest_size == 0 || src == NULL) {
        return false;
    }

    written = snprintf(dest, dest_size, "%s", src);
    return written >= 0 && (size_t)written < dest_size;
}

static bool cache_name_is_safe(const char *cache_name)
{
    if (cache_name == NULL || cache_name[0] == '\0') {
        return false;
    }

    return strchr(cache_name, '/') == NULL &&
           strchr(cache_name, '\\') == NULL &&
           strcmp(cache_name, ".") != 0 &&
           strcmp(cache_name, "..") != 0;
}

#ifdef _WIN32
static wchar_t *utf8_to_wide_alloc(const char *text)
{
    int length;
    wchar_t *wide;

    if (text == NULL) {
        return NULL;
    }

    length = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, text, -1, NULL, 0);
    if (length <= 0) {
        return NULL;
    }

    wide = (wchar_t *)malloc((size_t)length * sizeof(*wide));
    if (wide == NULL) {
        return NULL;
    }

    if (MultiByteToWideChar(CP_UTF8,
                            MB_ERR_INVALID_CHARS,
                            text,
                            -1,
                            wide,
                            length) <= 0) {
        free(wide);
        return NULL;
    }

    return wide;
}

static bool wide_to_utf8_buffer(const wchar_t *wide, char *out, size_t out_size)
{
    int required;

    if (wide == NULL || out == NULL || out_size == 0 || out_size > INT_MAX) {
        return false;
    }

    required = WideCharToMultiByte(CP_UTF8,
                                   WC_ERR_INVALID_CHARS,
                                   wide,
                                   -1,
                                   NULL,
                                   0,
                                   NULL,
                                   NULL);
    if (required <= 0 || (size_t)required > out_size) {
        return false;
    }

    return WideCharToMultiByte(CP_UTF8,
                               WC_ERR_INVALID_CHARS,
                               wide,
                               -1,
                               out,
                               (int)out_size,
                               NULL,
                               NULL) > 0;
}

static int create_directory_wide(const wchar_t *path)
{
    wchar_t *copy;
    wchar_t *cursor;
    size_t length;
    int result = 0;

    if (path == NULL || path[0] == L'\0') {
        return -1;
    }

    length = wcslen(path);
    copy = (wchar_t *)malloc((length + 1) * sizeof(*copy));
    if (copy == NULL) {
        return -1;
    }
    memcpy(copy, path, (length + 1) * sizeof(*copy));

    if (length > 0 && (copy[length - 1] == L'\\' || copy[length - 1] == L'/')) {
        copy[length - 1] = L'\0';
    }

    cursor = copy;
    if (length >= 3 && copy[1] == L':' &&
        (copy[2] == L'\\' || copy[2] == L'/')) {
        cursor = copy + 3;
    } else if (length >= 2 && copy[0] == L'\\' && copy[1] == L'\\') {
        int separators = 0;
        cursor = copy + 2;
        while (*cursor != L'\0' && separators < 2) {
            if (*cursor == L'\\' || *cursor == L'/') {
                separators++;
            }
            cursor++;
        }
    }

    for (; *cursor != L'\0'; cursor++) {
        if (*cursor == L'\\' || *cursor == L'/') {
            wchar_t saved = *cursor;
            *cursor = L'\0';
            if (_wmkdir(copy) != 0 && errno != EEXIST) {
                result = -1;
                *cursor = saved;
                break;
            }
            *cursor = saved;
        }
    }

    if (result == 0 && _wmkdir(copy) != 0 && errno != EEXIST) {
        result = -1;
    }

    free(copy);
    return result;
}

static FILE *open_file_utf8(const char *path, const wchar_t *mode)
{
    wchar_t *wide_path;
    FILE *file;

    wide_path = utf8_to_wide_alloc(path);
    if (wide_path == NULL) {
        return NULL;
    }

    file = _wfopen(wide_path, mode);
    free(wide_path);
    return file;
}

static void remove_file_utf8(const char *path)
{
    wchar_t *wide_path = utf8_to_wide_alloc(path);
    if (wide_path != NULL) {
        DeleteFileW(wide_path);
        free(wide_path);
    }
}

static bool replace_file_utf8(const char *source, const char *destination)
{
    wchar_t *wide_source = utf8_to_wide_alloc(source);
    wchar_t *wide_destination = utf8_to_wide_alloc(destination);
    bool result = false;

    if (wide_source != NULL && wide_destination != NULL) {
        result = MoveFileExW(wide_source,
                             wide_destination,
                             MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH) != FALSE;
    }

    free(wide_source);
    free(wide_destination);
    return result;
}

static bool file_exists_utf8(const char *path)
{
    wchar_t *wide_path = utf8_to_wide_alloc(path);
    DWORD attributes;

    if (wide_path == NULL) {
        return false;
    }

    attributes = GetFileAttributesW(wide_path);
    free(wide_path);
    return attributes != INVALID_FILE_ATTRIBUTES &&
           (attributes & FILE_ATTRIBUTE_DIRECTORY) == 0;
}

static int get_cache_path(char *cache_file,
                          size_t cache_file_size,
                          const char *cache_name)
{
    PWSTR local_app_data = NULL;
    wchar_t *wide_name = NULL;
    wchar_t *cache_directory = NULL;
    wchar_t *full_path = NULL;
    size_t root_length;
    size_t folder_length = wcslen(WAPP_FOLDER);
    size_t name_length;
    HRESULT hr;
    int result = 1;

    if (!cache_name_is_safe(cache_name)) {
        fprintf(stderr, "Invalid image cache name\n");
        return 1;
    }

    hr = SHGetKnownFolderPath(&FOLDERID_LocalAppData, 0, NULL, &local_app_data);
    if (FAILED(hr)) {
        fprintf(stderr, "SHGetKnownFolderPath failed with error %08lx\n",
                (unsigned long)hr);
        return 1;
    }

    wide_name = utf8_to_wide_alloc(cache_name);
    if (wide_name == NULL) {
        goto cleanup;
    }

    root_length = wcslen(local_app_data);
    name_length = wcslen(wide_name);

    cache_directory = (wchar_t *)malloc((root_length + 1 + folder_length + 1) *
                                        sizeof(*cache_directory));
    if (cache_directory == NULL) {
        goto cleanup;
    }

    memcpy(cache_directory,
           local_app_data,
           root_length * sizeof(*cache_directory));
    cache_directory[root_length] = L'\\';
    memcpy(cache_directory + root_length + 1,
           WAPP_FOLDER,
           (folder_length + 1) * sizeof(*cache_directory));

    if (create_directory_wide(cache_directory) != 0) {
        fprintf(stderr, "Failed to create Gotify cache directory\n");
        goto cleanup;
    }

    full_path = (wchar_t *)malloc((root_length + 1 + folder_length + 1 +
                                  name_length + 1) * sizeof(*full_path));
    if (full_path == NULL) {
        goto cleanup;
    }

    swprintf(full_path,
             root_length + 1 + folder_length + 1 + name_length + 1,
             L"%ls\\%ls",
             cache_directory,
             wide_name);

    if (!wide_to_utf8_buffer(full_path, cache_file, cache_file_size)) {
        fprintf(stderr, "Image cache path is too long\n");
        goto cleanup;
    }

    result = 0;

cleanup:
    free(full_path);
    free(cache_directory);
    free(wide_name);
    CoTaskMemFree(local_app_data);
    return result;
}
#else
static int create_directory(const char *path)
{
    char copy[FILENAME_MAX];
    char *cursor;
    size_t length;

    if (!copy_text(copy, sizeof(copy), path)) {
        return -1;
    }

    length = strlen(copy);
    if (length == 0) {
        return -1;
    }
    if (copy[length - 1] == '/') {
        copy[length - 1] = '\0';
    }

    for (cursor = copy + 1; *cursor != '\0'; cursor++) {
        if (*cursor == '/') {
            *cursor = '\0';
            if (mkdir(copy, S_IRWXU) != 0 && errno != EEXIST) {
                return -1;
            }
            *cursor = '/';
        }
    }

    if (mkdir(copy, S_IRWXU) != 0 && errno != EEXIST) {
        return -1;
    }

    return 0;
}

static FILE *open_file_utf8(const char *path, const char *mode)
{
    return fopen(path, mode);
}

static void remove_file_utf8(const char *path)
{
    remove(path);
}

static bool replace_file_utf8(const char *source, const char *destination)
{
    return rename(source, destination) == 0;
}

static bool file_exists_utf8(const char *path)
{
    struct stat status;
    return stat(path, &status) == 0 && S_ISREG(status.st_mode);
}

static int get_cache_path(char *cache_file,
                          size_t cache_file_size,
                          const char *cache_name)
{
    const char *xdg_cache_home;
    const char *home;
    char cache_directory[FILENAME_MAX];
    int written;

    if (!cache_name_is_safe(cache_name)) {
        fprintf(stderr, "Invalid image cache name\n");
        return 1;
    }

    xdg_cache_home = getenv("XDG_CACHE_HOME");
    if (xdg_cache_home != NULL && xdg_cache_home[0] == '/') {
        written = snprintf(cache_directory,
                           sizeof(cache_directory),
                           "%s/%s",
                           xdg_cache_home,
                           APP_FOLDER);
    } else {
        home = getenv("HOME");
        if (home == NULL || home[0] == '\0') {
            fprintf(stderr, "HOME is not set\n");
            return 1;
        }
        written = snprintf(cache_directory,
                           sizeof(cache_directory),
                           "%s/.cache/%s",
                           home,
                           APP_FOLDER);
    }

    if (written < 0 || (size_t)written >= sizeof(cache_directory)) {
        fprintf(stderr, "Image cache directory path is too long\n");
        return 1;
    }

    if (create_directory(cache_directory) != 0) {
        perror("mkdir");
        return 1;
    }

    written = snprintf(cache_file,
                       cache_file_size,
                       "%s/%s",
                       cache_directory,
                       cache_name);
    if (written < 0 || (size_t)written >= cache_file_size) {
        fprintf(stderr, "Image cache path is too long\n");
        return 1;
    }

    return 0;
}
#endif

#if LIBCURL
struct image_writer {
    FILE *file;
    size_t bytes_written;
    bool failed;
};

static size_t write_image_data(void *ptr, size_t size, size_t nmemb, void *userdata)
{
    struct image_writer *writer = (struct image_writer *)userdata;
    size_t requested;
    size_t written;

    if (writer == NULL || writer->file == NULL ||
        (size != 0 && nmemb > SIZE_MAX / size)) {
        return 0;
    }

    requested = size * nmemb;
    written = fwrite(ptr, 1, requested, writer->file);
    writer->bytes_written += written;
    if (written != requested) {
        writer->failed = true;
    }

    return written;
}

int get_image(const char *gotify_image_url,
              const char *gotify_token,
              const char *cache_name,
              char *outfilename,
              size_t outfilename_size)
{
    CURL *curl = NULL;
    struct curl_slist *headers = NULL;
    struct image_writer writer;
    CURLcode curl_result;
    long response_code = 0;
    char token_header[512];
    char temporary_path[FILENAME_MAX];
    int written;
    bool success = false;

    if (gotify_image_url == NULL || gotify_token == NULL ||
        outfilename == NULL || outfilename_size == 0) {
        return 1;
    }

    outfilename[0] = '\0';
    if (get_cache_path(outfilename, outfilename_size, cache_name) != 0) {
        return 1;
    }

    written = snprintf(temporary_path,
                       sizeof(temporary_path),
                       "%s.tmp",
                       outfilename);
    if (written < 0 || (size_t)written >= sizeof(temporary_path)) {
        fprintf(stderr, "Temporary image path is too long\n");
        outfilename[0] = '\0';
        return 1;
    }

#ifdef _WIN32
    writer.file = open_file_utf8(temporary_path, L"wb");
#else
    writer.file = open_file_utf8(temporary_path, "wb");
#endif
    writer.bytes_written = 0;
    writer.failed = false;
    if (writer.file == NULL) {
        fprintf(stderr, "Failed to open image cache file for writing\n");
        outfilename[0] = '\0';
        return 1;
    }

    curl = curl_easy_init();
    if (curl == NULL) {
        fprintf(stderr, "curl_easy_init() failed\n");
        goto cleanup;
    }

    written = snprintf(token_header,
                       sizeof(token_header),
                       "X-Gotify-Key: %s",
                       gotify_token);
    if (written < 0 || (size_t)written >= sizeof(token_header)) {
        fprintf(stderr, "Gotify token is too long for HTTP header\n");
        goto cleanup;
    }

    headers = curl_slist_append(headers, token_header);
    if (headers == NULL) {
        fprintf(stderr, "Failed to allocate curl headers\n");
        goto cleanup;
    }

    curl_easy_setopt(curl, CURLOPT_URL, gotify_image_url);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 5L);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "GotifyClientApp/1.0");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_image_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &writer);

    curl_result = curl_easy_perform(curl);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);

    if (fflush(writer.file) != 0 || ferror(writer.file)) {
        writer.failed = true;
    }

    if (curl_result == CURLE_OK &&
        response_code >= 200 && response_code < 300 &&
        !writer.failed && writer.bytes_written > 0) {
        success = true;
    } else {
        fprintf(stderr,
                "Image download failed: curl=%s HTTP=%ld bytes=%lu\n",
                curl_easy_strerror(curl_result),
                response_code,
                (unsigned long)writer.bytes_written);
    }

cleanup:
    if (writer.file != NULL) {
        fclose(writer.file);
        writer.file = NULL;
    }
    curl_slist_free_all(headers);
    if (curl != NULL) {
        curl_easy_cleanup(curl);
    }

    if (success && replace_file_utf8(temporary_path, outfilename)) {
        return 0;
    }

    if (success) {
        fprintf(stderr, "Failed to atomically replace cached image\n");
    }
    remove_file_utf8(temporary_path);

    if (file_exists_utf8(outfilename)) {
        return 0;
    }

    outfilename[0] = '\0';
    return 1;
}
#else
int get_image(const char *gotify_image_url,
              const char *gotify_token,
              const char *cache_name,
              char *outfilename,
              size_t outfilename_size)
{
    (void)gotify_image_url;
    (void)gotify_token;
    (void)cache_name;
    if (outfilename != NULL && outfilename_size > 0) {
        outfilename[0] = '\0';
    }
    return 1;
}
#endif
