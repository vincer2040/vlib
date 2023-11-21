cmake_minimum_required(VERSION 3.10)

project(vlib VERSION 0.1)

enable_testing()

add_subdirectory(tests)
set(INSTALL_PREFIX "/usr/local")

set(CMAKE_C_COMPILER "gcc")

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -std=c99 -Wall -Werror -pedantic -fstack-clash-protection -fstack-protector-all -fstack-protector-strong -Werror=format-security -Werror=implicit-function-declaration -pipe -O2")

set(CMAKE_C_FLAGS_RELEASE "-std=c99 -Wall -Werror -pedantic -fstack-clash-protection -fstack-protector-all -fstack-protector-strong -Werror=format-security -Werror=implicit-function-declaration -pipe -O2 -s -DNDEBUG")

add_library(
    vlib
    src/binary_search.c
    src/bubble_sort.c
    src/insertion_sort.c
    src/merge_sort.c
    src/quick_sort.c
    src/vec.c
    src/queue.c
    src/list.c
    src/pq.c
    src/siphash.c
    src/sha256.c
    src/util.c
    src/ht.c
    src/vstr.c
    src/binary_tree.c
    src/binary_node.c
    src/bt_pre_order.c
    src/bt_in_order.c
    src/bt_post_order.c
    src/avl_tree.c
    src/bt_bfs.c
    src/bt_compare.c
    src/tree.c
    src/lru.c
)

install(FILES build/libvlib.a DESTINATION "${INSTALL_PREFIX}/lib")
install(FILES src/vlib.h DESTINATION "${INSTALL_PREFIX}/include")
