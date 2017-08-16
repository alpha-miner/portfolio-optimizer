file(GLOB_RECURSE MEAN_VARIANCE_FILES "*.hpp" "*.cpp")
add_executable(mean_variance ${MEAN_VARIANCE_FILES})

target_link_libraries(mean_variance pfopt)