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
