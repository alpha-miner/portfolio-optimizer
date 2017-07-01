cmake_minimum_required (VERSION 3.0)

project (pfopt)

find_package (Eigen3 3.3 REQUIRED NO_MODULE)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../Ipopt/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../eigen/include)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../OpenBlAS/lib)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../Ipopt/lib)

add_subdirectory(pfopt)
add_subdirectory(examples)


