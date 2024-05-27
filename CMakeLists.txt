cmake_minimum_required(VERSION 3.10)
project(GotifyClientApp)

set(CMAKE_C_STANDARD 11)

find_package(PkgConfig REQUIRED)
#pkg_check_modules(LIBNOTIFY REQUIRED libnotify)
pkg_check_modules(LIBWEBSOCKETS REQUIRED libwebsockets)

pkg_check_modules(LIBSSL REQUIRED libssl)
pkg_check_modules(LIBCRYPTO REQUIRED libcrypto)
pkg_check_modules(LIBUV REQUIRED libuv-static)
pkg_check_modules(LIBCAP REQUIRED libcap)

add_executable(GotifyClientApp src/main.c)

target_compile_options(GotifyClientApp PRIVATE -DLIBWEBSOCKETS)

target_include_directories(GotifyClientApp PRIVATE ${LIBNOTIFY_INCLUDE_DIRS} ${LIBWEBSOCKETS_INCLUDE_DIRS})
#target_link_libraries(GotifyClientApp PRIVATE ${LIBNOTIFY_LIBRARIES} ${LIBWEBSOCKETS_LIBRARIES})
target_link_libraries(GotifyClientApp PRIVATE ${LIBNOTIFY_LIBRARIES} ${LIBWEBSOCKETS_LIBRARIES}.a ${LIBUV_LIBRARIES}.a ${LIBCAP_LIBRARIES}.a ${LIBSSL_LIBRARIES} ${LIBCRYPTO_LIBRARIES} ev.a m)

if (WIN32)
    target_link_libraries(GotifyClientApp PRIVATE ws2_32)
endif()
