file(GLOB_RECURSE MEAN_VARIANCE_FILES "*.hpp" "*.cpp")
add_executable(mean_variance ${MEAN_VARIANCE_FILES})

target_link_libraries(mean_variance pfopt)
target_link_libraries(mean_variance ${Boost_CHRONO_LIBRARY})
target_link_libraries(mean_variance ${Boost_SYSTEM_LIBRARY})