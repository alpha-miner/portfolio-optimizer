file(GLOB_RECURSE PFOPT_FILES "*.hpp" "*.cpp")

if(MSVC)
    add_library(pfopt STATIC ${PFOPT_FILES})
else()
    add_library(pfopt SHARED ${PFOPT_FILES})
endif()

target_link_libraries(pfopt alglib)

if(MSVC)
    target_link_libraries(pfopt libCoinUtils)
    target_link_libraries(pfopt libClp)
    target_link_libraries(pfopt Ipopt-vc8)
    target_link_libraries(pfopt IpOptFSS)

    add_custom_command(TARGET pfopt POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${IPOPT_LIB_DIR}/Ipopt-vc8.dll
            ${LIBRARY_BIN_OUTOUT_DIRECTORY})

    add_custom_command(TARGET pfopt POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${IPOPT_LIB_DIR}/IpOptFSS.dll
            ${LIBRARY_BIN_OUTOUT_DIRECTORY})

    add_custom_command(TARGET pfopt POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${IPOPT_LIB_DIR}/libhsl.dll
            ${LIBRARY_BIN_OUTOUT_DIRECTORY})
else()
    target_link_libraries(pfopt Clp)
    target_link_libraries(pfopt ClpSolver)
    target_link_libraries(pfopt CoinUtils)
    target_link_libraries(pfopt Osi)
    target_link_libraries(pfopt OsiClp)
    target_link_libraries(pfopt ipopt)
endif()