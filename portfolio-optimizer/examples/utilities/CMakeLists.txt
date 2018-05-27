file(GLOB_RECURSE UTILITIES_FILES "*.hpp" "*.cpp")
add_executable(calc_grad ${UTILITIES_FILES})

target_link_libraries(calc_grad pfopt)