file(GLOB_RECURSE PFOPT_FILES "*.hpp" "*.cpp")
add_library(pfopt ${LIB_TYPE} ${PFOPT_FILES})