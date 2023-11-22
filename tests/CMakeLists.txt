include(FetchContent)

set(LIBCHECK_VERSION "0.15.2")

# Use FetchContent to download and include libcheck
FetchContent_Declare(
    libcheck
    URL https://github.com/libcheck/check/archive/${LIBCHECK_VERSION}.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP true  # Specify the option here
)

FetchContent_MakeAvailable(libcheck)

# vstr
add_executable(vstr_test vstr_test.c)

target_link_libraries(vstr_test PUBLIC vlib check pthread)

target_include_directories(vstr_test PUBLIC "${PROJECT_BINARY_DIR}")

add_test(NAME vstr_test COMMAND vstr_test WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/Testing)
set_tests_properties(vstr_test PROPERTIES TIMEOUT 30)

# vec
add_executable(vec_test vec_test.c)

target_link_libraries(vec_test PUBLIC vlib check pthread)

target_include_directories(vec_test PUBLIC "${PROJECT_BINARY_DIR}")

add_test(NAME vec_test COMMAND vec_test WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/Testing)
set_tests_properties(vec_test PROPERTIES TIMEOUT 30)

# queue
add_executable(queue_test queue_test.c)

target_link_libraries(queue_test PUBLIC vlib check pthread)

target_include_directories(queue_test PUBLIC "${PROJECT_BINARY_DIR}")

add_test(NAME queue_test COMMAND queue_test WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/Testing)
set_tests_properties(queue_test PROPERTIES TIMEOUT 30)

# binary search
add_executable(binary_search_test binary_search_test.c)

target_link_libraries(binary_search_test PUBLIC vlib check pthread)

target_include_directories(binary_search_test PUBLIC "${PROJECT_BINARY_DIR}")

add_test(NAME binary_search_test COMMAND binary_search_test WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/Testing)
set_tests_properties(binary_search_test PROPERTIES TIMEOUT 30)

# bubble sort
add_executable(bubble_sort_test bubble_sort_test.c)

target_link_libraries(bubble_sort_test PUBLIC vlib check pthread)

target_include_directories(bubble_sort_test PUBLIC "${PROJECT_BINARY_DIR}")

add_test(NAME bubble_sort_test COMMAND bubble_sort_test WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/Testing)
set_tests_properties(bubble_sort_test PROPERTIES TIMEOUT 30)

# insertion sort
add_executable(insertion_sort_test insertion_sort_test.c)

target_link_libraries(insertion_sort_test PUBLIC vlib check pthread)

target_include_directories(insertion_sort_test PUBLIC "${PROJECT_BINARY_DIR}")

add_test(NAME insertion_sort_test COMMAND insertion_sort_test WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/Testing)
set_tests_properties(insertion_sort_test PROPERTIES TIMEOUT 30)

# merge sort
add_executable(merge_sort_test merge_sort_test.c)

target_link_libraries(merge_sort_test PUBLIC vlib check pthread)

target_include_directories(merge_sort_test PUBLIC "${PROJECT_BINARY_DIR}")

add_test(NAME merge_sort_test COMMAND merge_sort_test WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/Testing)
set_tests_properties(merge_sort_test PROPERTIES TIMEOUT 30)

# quick sort
add_executable(quick_sort_test quick_sort_test.c)

target_link_libraries(quick_sort_test PUBLIC vlib check pthread)

target_include_directories(quick_sort_test PUBLIC "${PROJECT_BINARY_DIR}")

add_test(NAME quick_sort_test COMMAND quick_sort_test WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/Testing)
set_tests_properties(quick_sort_test PROPERTIES TIMEOUT 30)

# list
add_executable(list_test list_test.c)

target_link_libraries(list_test PUBLIC vlib check pthread)

target_include_directories(list_test PUBLIC "${PROJECT_BINARY_DIR}")

add_test(NAME list_test COMMAND list_test WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/Testing)
set_tests_properties(list_test PROPERTIES TIMEOUT 30)

# tree traversal
add_executable(tree_traversal_test tree_traversal_test.c)

target_link_libraries(tree_traversal_test PUBLIC vlib check pthread)

target_include_directories(tree_traversal_test PUBLIC "${PROJECT_BINARY_DIR}")

add_test(NAME tree_traversal_test COMMAND tree_traversal_test WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/Testing)
set_tests_properties(tree_traversal_test PROPERTIES TIMEOUT 30)

# bfs
add_executable(bfs_test bfs_test.c)

target_link_libraries(bfs_test PUBLIC vlib check pthread)

target_include_directories(bfs_test PUBLIC "${PROJECT_BINARY_DIR}")

add_test(NAME bfs_test COMMAND bfs_test WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/Testing)
set_tests_properties(bfs_test PROPERTIES TIMEOUT 30)

# bt compare
add_executable(bt_compare_test bt_compare_test.c)

target_link_libraries(bt_compare_test PUBLIC vlib check pthread)

target_include_directories(bt_compare_test PUBLIC "${PROJECT_BINARY_DIR}")

add_test(NAME bt_compare_test COMMAND bt_compare_test WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/Testing)
set_tests_properties(bt_compare_test PROPERTIES TIMEOUT 30)

# pq
add_executable(pq_test pq_test.c)

target_link_libraries(pq_test PUBLIC vlib check pthread)

target_include_directories(pq_test PUBLIC "${PROJECT_BINARY_DIR}")

add_test(NAME pq_test COMMAND pq_test WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/Testing)
set_tests_properties(pq_test PROPERTIES TIMEOUT 30)

# ht
add_executable(ht_test ht_test.c)

target_link_libraries(ht_test PUBLIC vlib check pthread)

target_include_directories(ht_test PUBLIC "${PROJECT_BINARY_DIR}")

add_test(NAME ht_test COMMAND ht_test WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/Testing)
set_tests_properties(ht_test PROPERTIES TIMEOUT 30)

# avl tree
add_executable(avl_test avl_test.c)

target_link_libraries(avl_test PUBLIC vlib check pthread)

target_include_directories(avl_test PUBLIC "${PROJECT_BINARY_DIR}")

add_test(NAME avl_test COMMAND avl_test WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/Testing)
set_tests_properties(avl_test PROPERTIES TIMEOUT 30)

# tree
add_executable(tree_test tree_test.c)

target_link_libraries(tree_test PUBLIC vlib check pthread)

target_include_directories(tree_test PUBLIC "${PROJECT_BINARY_DIR}")

add_test(NAME tree_test COMMAND tree_test WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/Testing)
set_tests_properties(tree_test PROPERTIES TIMEOUT 30)

# lru
add_executable(lru_test lru_test.c)

target_link_libraries(lru_test PUBLIC vlib check pthread)

target_include_directories(lru_test PUBLIC "${PROJECT_BINARY_DIR}")

add_test(NAME lru_test COMMAND lru_test WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/Testing)
set_tests_properties(lru_test PROPERTIES TIMEOUT 30)
