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

#ifndef _GCA_MAIN_H_
#define _GCA_MAIN_H_

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

int get_image(char *gotify_image_url, char *gotify_token, char *outfilename);

#endif /* _GCA_MAIN_H_ */
