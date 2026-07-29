/**
 * @copyright Copyright (c) 2024, Mohamed-Ismail MJRI.
 *
 * @author Mohamed-Ismail MJRI <imejri@hotmail.com>
 *
 * @license AGPL-3.0
 */
#ifndef _GCA_MAIN_H_
#define _GCA_MAIN_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if LIBCURL
#include <curl/curl.h>
#endif

#ifdef _WIN32
#include <windows.h>
#include <shlobj.h>
#endif

int get_image(const char *gotify_image_url,
              const char *gotify_token,
              const char *cache_name,
              char *outfilename,
              size_t outfilename_size);

#endif /* _GCA_MAIN_H_ */
