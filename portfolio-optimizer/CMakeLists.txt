cmake_minimum_required (VERSION 3.0)

project (pfopt)

find_package (Eigen3 3.3 REQUIRED NO_MODULE)
find_package(Boost REQUIRED COMPONENTS chrono system)

if(MSVC)
   include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../eigen
else()
    add_definitions(-std=c++11)

    include_directories(${CMAKE_CURRENT_SOURCE_DIR})
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../Ipopt/include)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../eigen/include/eigen3)

    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../OpenBLAS/lib)
    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../Ipopt/lib)

    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/lib)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/lib)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/bin)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/bin)

endif()


add_subdirectory(pfopt)
add_subdirectory(examples)


