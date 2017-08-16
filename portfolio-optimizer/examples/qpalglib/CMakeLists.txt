file(GLOB_RECURSE QP_ALGLIB_FILES "*.hpp" "*.cpp")
add_executable(qp_alglib ${QP_ALGLIB_FILES})

target_link_libraries(qp_alglib pfopt)
