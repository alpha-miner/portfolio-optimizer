file(GLOB_RECURSE LOAD_CSVC_FILES "*.hpp" "*.cpp")
add_executable(load_csv ${LOAD_CSVC_FILES})
target_link_libraries(load_csv pfopt)