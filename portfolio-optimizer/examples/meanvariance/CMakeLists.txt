file(GLOB_RECURSE MEAN_VARIANCE_FILES "*.hpp" "*.cpp")
add_executable(mean_variance ${MEAN_VARIANCE_FILES})

if(MSVC)
    target_link_libraries(mean_variance Ipopt-vc8)
    target_link_libraries(mean_variance IpOptFSS)
else()
    target_link_libraries(mean_variance ipopt)
endif()

target_link_libraries(mean_variance pfopt)
target_link_libraries(mean_variance ${Boost_CHRONO_LIBRARY})
target_link_libraries(mean_variance ${Boost_SYSTEM_LIBRARY})

if(MSVC)

add_custom_command(TARGET mean_variance POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${IPOPT_LIB_DIR}/Ipopt-vc8.dll
        ${LIBRARY_BIN_OUTOUT_DIRECTORY})

add_custom_command(TARGET mean_variance POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${IPOPT_LIB_DIR}/IpOptFSS.dll
        ${LIBRARY_BIN_OUTOUT_DIRECTORY})

add_custom_command(TARGET mean_variance POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${IPOPT_LIB_DIR}/libhsl.dll
        ${LIBRARY_BIN_OUTOUT_DIRECTORY})
endif()