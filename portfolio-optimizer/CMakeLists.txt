cmake_minimum_required (VERSION 3.0)

project (pfopt)

find_package (Eigen3 3.3 REQUIRED NO_MODULE)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

if(MSVC)
    set(LIB_TYPE STATIC)
else()
    set(LIB_TYPE SHARED)
endif()

add_subdirectory(pfopt)
add_subdirectory(examples)


