cmake_minimum_required (VERSION 3.0)

project (pfopt)

find_package (Eigen3 3.3 REQUIRED NO_MODULE)
find_package(Boost REQUIRED COMPONENTS chrono system)

if(MSVC)
else()
    add_definitions(-std=c++11)

    include_directories(${CMAKE_CURRENT_SOURCE_DIR})
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../Ipopt/include)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../eigen/include)

    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../OpenBLAS/lib)
    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../Ipopt/lib)

endif()


add_subdirectory(pfopt)
add_subdirectory(examples)


