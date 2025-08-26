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

#include <sys/stat.h>
#include <errno.h>
#include "main.h"


#ifdef _WIN32
#include <direct.h>     /* _mkdir, _wmkdir */
#define WAPP_FOLDER     L"\\GotifyClientApp"

/* ------------------------------------------------------------------ */
/* 1. file/directory existence test ---------------------------------- */
int check_file_exists(const char *filename)
{
    /* Convert UTF-8 to UTF-16 and ask the OS */
    WCHAR wpath[MAX_PATH];
    MultiByteToWideChar(CP_UTF8, 0, filename, -1, wpath, MAX_PATH);

    DWORD attribs = GetFileAttributesW(wpath);
    return (attribs != INVALID_FILE_ATTRIBUTES);
}

/* ------------------------------------------------------------------ */
/* 2. Create directory tree ------------------------------------------ */
static int create_directory_one(const WCHAR *wpath)
{
    /* _wmkdir returns 0 on success, -1 on error */
    if (_wmkdir(wpath) == 0)               /* created */
        return 0;
    if (errno == EEXIST)                   /* already there */
        return 0;
    return -1;
}

int create_directory(const char *path)
{
    WCHAR temp[MAX_PATH];
    MultiByteToWideChar(CP_UTF8, 0, path, -1, temp, MAX_PATH);

    /* Strip trailing back-slash */
    size_t len = wcslen(temp);
    if (len && (temp[len - 1] == L'\\' || temp[len - 1] == L'/'))
        temp[len - 1] = 0;

    /* Walk through the path, creating as we go */
    for (WCHAR *p = temp + 1; *p; ++p)
    {
        if (*p == L'\\' || *p == L'/')
        {
            *p = 0;
            if (create_directory_one(temp) != 0)
                return -1;
            *p = L'\\';               /* always use back-slash on Win32 */
        }
    }
    if (create_directory_one(temp) != 0)
        return -1;
    return 0;
}

/* ------------------------------------------------------------------ */
/* 3. Check directory exists, optionally create ---------------------- */
int check_directory_exists(char *file_path, int create)
{
    char dir[MAX_PATH];
    char *last_slash;

    strcpy(dir, file_path);
    last_slash = strrchr(dir, '\\');      /* Windows separator */
    if (!last_slash)
        last_slash = strrchr(dir, '/');   /* Accept / as well */

    if (last_slash != NULL)
    {
        *last_slash = '\0';
        if (!check_file_exists(dir))
        {
            printf("Directory %s does not exist.\n", dir);
            if (create)
            {
                printf("Creating...\n");
                if (create_directory(dir) != 0)
                {
                    printf("Failed to create directory %s\n", dir);
                    return -1;
                }
            }
            else
                return -1;
        }
    }
    return 0;
}
#else
#define APP_FOLDER      "GotifyClientApp"
// Function to check if a file exists
int check_file_exists(const char *filename) {
    struct stat buffer;
    return (stat(filename, &buffer) == 0);
}

// Function to create directories recursively
int create_directory(const char *path) {
    char temp[FILENAME_MAX];
    char *pos = NULL;
    size_t len;

    snprintf(temp, sizeof(temp), "%s", path);
    len = strlen(temp);
    if (temp[len - 1] == '/') {
        temp[len - 1] = '\0';
    }

    for (pos = temp + 1; *pos; pos++) {
        if (*pos == '/') {
            *pos = '\0';
            if (mkdir(temp, S_IRWXU) && errno != EEXIST) {
                perror("mkdir");
                return -1;
            }
            *pos = '/';
        }
    }
    if (mkdir(temp, S_IRWXU) && errno != EEXIST) {
        perror("mkdir");
        return -1;
    }
    return 0;
}

int check_directory_exists(char *file_path, int create)
{
    char dir[FILENAME_MAX];
    char *last_slash;
    strcpy(dir, file_path);
    last_slash = strrchr(dir, '/');
    if (last_slash != NULL) {
        *last_slash = '\0';
        if (!check_file_exists(dir)) {
            printf("Directory %s does not exist.", dir);
            if (create) {
                printf("Creating...");
                if (create_directory(dir) != 0) {
                    printf("Failed to create directory %s", dir);
                    return -1;
                }
            } else {
                return -1;
            }
        }
    }
    return 0;
}
#endif

#ifdef _WIN32
int get_cache_path(char *cache_file, char *file) {
    PWSTR path = NULL;
    HRESULT hr = SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &path);
    if (FAILED(hr)) {
        fprintf(stderr, "SHGetKnownFolderPath failed with error %08x\n", hr);
        return 1;
    }

    // Append the cache directory to the path
    size_t len = wcslen(path);
    size_t wafl = wcslen(WAPP_FOLDER);
    WCHAR* cache_dir = malloc((len + wafl) * sizeof(WCHAR));
    if (!cache_dir) {
        CoTaskMemFree(path);
        return 1;
    }
    wcscpy_s(cache_dir, len + wafl, path);
    wcscpy_s(cache_dir + len, wafl, WAPP_FOLDER);

    printf("Cache directory: %ls\n", cache_dir);
    snprintf(cache_file, FILENAME_MAX, "%ls\\%s", cache_dir, file);
    check_directory_exists(cache_file, 1);

    free(cache_dir);
    CoTaskMemFree(path);

    return 0;
}
#else
int get_cache_path(char *cache_file, char *file) {
    char* home_dir = getenv("HOME");
    if (home_dir == NULL) {
        perror("getenv");
        return EXIT_FAILURE;
    }

    // Constructing the cache directory path
    snprintf(cache_file, FILENAME_MAX, "%s/.cache/%s/%s", home_dir, APP_FOLDER, file);
    check_directory_exists(cache_file, 1);

    printf("Cache file: %s\n", cache_file);

    return 0;
}
#endif

char* get_last_element(char* uri_path) {
    char* last_slash = strrchr(uri_path, '/');
    if (last_slash == NULL) {
        last_slash = strrchr(uri_path, '\\'); // Try backslash for Windows
    }
    if (last_slash == NULL) {
        return (char*)uri_path; // No slashes found, return the entire string
    }
    static char last_element[256]; // Allocate a buffer to store the last element
    strcpy(last_element, last_slash + 1); // Copy the substring starting from the last slash + 1
    return last_element;
}

#if LIBCURL
// Callback function to write data received from cURL to a file
size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    return fwrite(ptr, size, nmemb, stream);
}

int get_image(char *gotify_image_url, char *gotify_token, char *outfilename) {
    CURL *curl;
    CURLcode res;
    FILE *fp;

    if (get_cache_path(outfilename, get_last_element(gotify_image_url))) {
        return 1;
    }
    // Initialize libcurl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if(curl) {
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        char token_header[256];
        snprintf(token_header, sizeof(token_header), "X-Gotify-Key: %s", gotify_token);
        headers = curl_slist_append(headers, token_header);
        // Set the URL of the image
        curl_easy_setopt(curl, CURLOPT_URL, gotify_image_url);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Open a file to save the downloaded image
        fp = fopen(outfilename, "wb");
        if(fp == NULL) {
            fprintf(stderr, "Failed to open file for writing\n");
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            return 1;
        }

        // Set the callback function to write data to the file
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

        // Perform the request
        res = curl_easy_perform(curl);

        // Check for errors
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        // Cleanup
        fclose(fp);
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
}
#endif
