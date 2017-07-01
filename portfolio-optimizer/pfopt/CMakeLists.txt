file(GLOB_RECURSE PFOPT_FILES "*.hpp" "*.cpp")
add_library(pfopt SHARED ${PFOPT_FILES})