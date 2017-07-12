file(GLOB_RECURSE QP_ALGLIB_FILES "*.hpp" "*.cpp")
add_executable(qp_alglib ${QP_ALGLIB_FILES})

target_link_libraries(qp_alglib pfopt)
target_link_libraries(qp_alglib pfopt)
target_link_libraries(qp_alglib ${Boost_CHRONO_LIBRARY})
target_link_libraries(qp_alglib ${Boost_SYSTEM_LIBRARY})
