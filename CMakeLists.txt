cmake_minimum_required(VERSION 3.10)
project(GotifyClientApp)

set(CMAKE_C_STANDARD 11)

# --- Dependencies ---
if(WIN32)
    # Use vcpkg integration
    find_package(CURL REQUIRED)
    find_package(libwebsockets CONFIG REQUIRED)
else()
    # Use pkg-config on Linux
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(LIBCURL REQUIRED libcurl)
    pkg_check_modules(LIBWEBSOCKETS REQUIRED libwebsockets)
    pkg_check_modules(LIBNOTIFY libnotify)
    pkg_check_modules(LIBSSL libssl)
    pkg_check_modules(LIBCRYPTO libcrypto)
    pkg_check_modules(LIBUV libuv-static)
    pkg_check_modules(LIBCAP libcap)
endif()

# --- Sources ---
add_library(cJSON_LIBRARIES OBJECT src/lib/cJSON.c)
add_library(file_LIBRARIES OBJECT src/file.c)
add_executable(GotifyClientApp
    $<TARGET_OBJECTS:cJSON_LIBRARIES>
    $<TARGET_OBJECTS:file_LIBRARIES>
    src/main.c
)

if(WIN32)
    target_compile_options(file_LIBRARIES PRIVATE -DLIBCURL=${LIBCURL_FOUND}1)
    target_compile_options(GotifyClientApp PRIVATE -DLIBWEBSOCKETS=${LIBWEBSOCKETS_FOUND}1 -DLIBNOTIFY=${LIBNOTIFY_FOUND}0 -DLIBCURL=${LIBCURL_FOUND}1)
else()
    target_compile_options(file_LIBRARIES PRIVATE -DLIBCURL=${LIBCURL_FOUND}0)
    target_compile_options(GotifyClientApp PRIVATE -DLIBWEBSOCKETS=${LIBWEBSOCKETS_FOUND}0 -DLIBNOTIFY=${LIBNOTIFY_FOUND}0 -DLIBCURL=${LIBCURL_FOUND}0)
endif()
# --- Include dirs ---
if(WIN32)
    # Handled by imported targets
else()
    target_include_directories(GotifyClientApp PRIVATE
        ${LIBCURL_INCLUDE_DIRS}
        ${LIBWEBSOCKETS_INCLUDE_DIRS}
        ${LIBNOTIFY_INCLUDE_DIRS}
    )
endif()

# --- Linking ---
if(WIN32)
    target_link_libraries(GotifyClientApp PRIVATE
        CURL::libcurl
        websockets_shared
        shell32
        user32
        ws2_32
    )
else()
    #target_link_libraries(GotifyClientApp PRIVATE ${LIBNOTIFY_LIBRARIES} ${LIBWEBSOCKETS_LIBRARIES} ${LIBCURL_LIBRARIES})
    target_link_libraries(GotifyClientApp PRIVATE
        ${LIBCURL_LIBRARIES}
        ${LIBWEBSOCKETS_LIBRARIES}.a
        ${LIBNOTIFY_LIBRARIES}
        ${LIBUV_LIBRARIES}.a
        ${LIBCAP_LIBRARIES}.a
        ${LIBSSL_LIBRARIES}
        ${LIBCRYPTO_LIBRARIES}
        ev.a m
    )
endif()
