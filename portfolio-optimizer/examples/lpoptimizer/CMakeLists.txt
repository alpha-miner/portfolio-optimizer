file(GLOB_RECURSE LP_OPTIMIZER_FILES "*.hpp" "*.cpp")
add_executable(lp_optimizer ${LP_OPTIMIZER_FILES})

target_link_libraries(lp_optimizer pfopt)
target_link_libraries(lp_optimizer ${Boost_CHRONO_LIBRARY})
target_link_libraries(lp_optimizer ${Boost_SYSTEM_LIBRARY})