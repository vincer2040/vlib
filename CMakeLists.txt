cmake_minimum_required(VERSION 3.10)

project(vlib VERSION 0.1)

enable_testing()

add_subdirectory(tests)
set(INSTALL_PREFIX "/usr/local")

set(CMAKE_C_COMPILER "gcc")

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -std=c99 -Wall -Werror -pedantic -fstack-clash-protection -fstack-protector-all -fstack-protector-strong -Werror=format-security -Werror=implicit-function-declaration -pipe -O2")

set(CMAKE_C_FLAGS_RELEASE "-std=c99 -Wall -Werror -pedantic -fstack-clash-protection -fstack-protector-all -fstack-protector-strong -Werror=format-security -Werror=implicit-function-declaration -pipe -O2 -s -DNDEBUG")

add_library(vlib src/vec.c)

install(FILES build/libvlib.a DESTINATION "${INSTALL_PREFIX}/lib")
install(FILES src/vlib.h DESTINATION "${INSTALL_PREFIX}/include")
