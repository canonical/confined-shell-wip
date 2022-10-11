cmake_minimum_required(VERSION 3.16)
project(desktop-launch)
include(FindPkgConfig)

set(CMAKE_CXX_STANDARD 20)
pkg_check_modules(GIO REQUIRED gio-2.0 gio-unix-2.0)

add_executable(env env-main.cpp)

target_include_directories(env PRIVATE ${GIO_INCLUDE_DIRS})

target_link_libraries(env ${GIO_LDFLAGS})

install(PROGRAMS ${CMAKE_BINARY_DIR}/env
    DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
)
