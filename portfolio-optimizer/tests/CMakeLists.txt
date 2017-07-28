file(GLOB_RECURSE TEST_FILES "*.hpp" "*.cpp")

add_executable(test_suite ${TEST_FILES})

target_link_libraries(test_suite pfopt)
target_link_libraries(test_suite alglib)
target_link_libraries(test_suite gtest)
target_link_libraries(test_suite gtest_main)