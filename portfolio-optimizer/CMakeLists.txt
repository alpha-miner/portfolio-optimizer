cmake_minimum_required (VERSION 3.0)

project (pfopt)

if(MSVC)
    set(USE_STATIC_BOOST ON)
    set(Boost_USE_STATIC_LIBS ON)
endif()

find_package(Boost REQUIRED COMPONENTS chrono system)

add_definitions(-DHAVE_CONFIG_H)

if (Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
endif (Boost_FOUND)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

if(MSVC)
    add_definitions(-DPFOPF_EXPORTS)

    set(IPOPT_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../Ipopt/windows/lib/${CMAKE_BUILD_TYPE})
    set(ALGLIB_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../alglib/lib)
    set(BOOST_LIB_DIR $ENV{BOOST_ROOT}/stage/lib)
    set(LIBRARY_BIN_OUTOUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../alglib/src)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../eigen)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../Ipopt/windows/include)
    link_directories(${IPOPT_LIB_DIR})
    link_directories(${BOOST_LIB_DIR})
    link_directories(${ALGLIB_LIB_DIR})

    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${LIBRARY_BIN_OUTOUT_DIRECTORY})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${LIBRARY_BIN_OUTOUT_DIRECTORY})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${LIBRARY_BIN_OUTOUT_DIRECTORY})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${LIBRARY_BIN_OUTOUT_DIRECTORY})

else()
    add_definitions(-std=c++11)

    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../Ipopt/include)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../eigen/include/eigen3)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../alglib/src)

    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../OpenBLAS/lib)
    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../Ipopt/lib)
    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../alglib/lib)

    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/lib)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/lib)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/bin)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/bin)

endif()

add_subdirectory(pfopt)
add_subdirectory(examples)


