file(GLOB_RECURSE PFOPT_FILES "*.hpp" "*.cpp")
add_library(pfopt SHARED ${PFOPT_FILES})
target_link_libraries(pfopt alglib)