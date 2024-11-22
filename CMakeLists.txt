cmake_minimum_required(VERSION 3.10)
project(GotifyClientApp)

set(CMAKE_C_STANDARD 11)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBNOTIFY libnotify)
pkg_check_modules(LIBCURL libcurl)
pkg_check_modules(LIBWEBSOCKETS REQUIRED libwebsockets)

pkg_check_modules(LIBSSL REQUIRED libssl)
pkg_check_modules(LIBCRYPTO REQUIRED libcrypto)
pkg_check_modules(LIBUV REQUIRED libuv-static)
pkg_check_modules(LIBCAP REQUIRED libcap)

add_library(cJSON_LIBRARIES OBJECT src/lib/cJSON.c)
add_library(file_LIBRARIES OBJECT src/file.c)
add_executable(GotifyClientApp $<TARGET_OBJECTS:cJSON_LIBRARIES> $<TARGET_OBJECTS:file_LIBRARIES> src/main.c)

target_compile_options(file_LIBRARIES PRIVATE -DLIBCURL=${LIBCURL_FOUND}0)
target_compile_options(GotifyClientApp PRIVATE -DLIBWEBSOCKETS=${LIBWEBSOCKETS_FOUND}0 -DLIBNOTIFY=${LIBNOTIFY_FOUND}0 -DLIBCURL=${LIBCURL_FOUND}0)

target_include_directories(GotifyClientApp PRIVATE ${LIBNOTIFY_INCLUDE_DIRS} ${LIBWEBSOCKETS_INCLUDE_DIRS} ${LIBCURL_INCLUDE_DIRS})
#target_link_libraries(GotifyClientApp PRIVATE ${LIBNOTIFY_LIBRARIES} ${LIBWEBSOCKETS_LIBRARIES})
target_link_libraries(GotifyClientApp PRIVATE ${LIBCURL_LIBRARIES} ${LIBNOTIFY_LIBRARIES} ${LIBWEBSOCKETS_LIBRARIES}.a ${LIBUV_LIBRARIES}.a ${LIBCAP_LIBRARIES}.a ${LIBSSL_LIBRARIES} ${LIBCRYPTO_LIBRARIES} ev.a m)

if (WIN32)
    target_link_libraries(GotifyClientApp PRIVATE ws2_32)
endif()
