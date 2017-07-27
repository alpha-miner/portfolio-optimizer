file(GLOB_RECURSE TEST_FILES "*.hpp" "*.cpp")

add_executable(test_suite ${TEST_FILES})

target_link_libraries(test_suite pfopt)
target_link_libraries(test_suite alglib)
target_link_libraries(test_suite gtest)
target_link_libraries(test_suite gtest_main)


if(MSVC)
    add_custom_command(TARGET test_suite POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${IPOPT_LIB_DIR}/Ipopt-vc8.dll
            ${LIBRARY_BIN_OUTOUT_DIRECTORY})

    add_custom_command(TARGET test_suite POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${IPOPT_LIB_DIR}/IpOptFSS.dll
            ${LIBRARY_BIN_OUTOUT_DIRECTORY})

    add_custom_command(TARGET test_suite POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${IPOPT_LIB_DIR}/libhsl.dll
            ${LIBRARY_BIN_OUTOUT_DIRECTORY})
else()
    target_link_libraries(test_suite pthread)
endif()