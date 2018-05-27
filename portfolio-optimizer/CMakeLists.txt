cmake_minimum_required (VERSION 2.8)

project (pfopt)

if(MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD /openmp")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd /openmp")
else()
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fopenmp -march=native")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fopenmp -march=native")
endif()

add_definitions(-DHAVE_CONFIG_H)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

if(MSVC)
    set(IPOPT_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../Ipopt/windows/lib)
    set(CLP_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../clp/windows/lib/${CMAKE_BUILD_TYPE})
    set(ALGLIB_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../alglib/lib)
    set(BOOST_LIB_DIR $ENV{BOOST_ROOT}/stage/lib)
    set(LIBRARY_BIN_OUTOUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../alglib/src)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../eigen)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../Ipopt/windows/include)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../clp/windows/include)
    
    link_directories(${IPOPT_LIB_DIR})
    link_directories(${CLP_LIB_DIR})
    link_directories(${BOOST_LIB_DIR})
    link_directories(${ALGLIB_LIB_DIR})

    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${LIBRARY_BIN_OUTOUT_DIRECTORY})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${LIBRARY_BIN_OUTOUT_DIRECTORY})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${LIBRARY_BIN_OUTOUT_DIRECTORY})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${LIBRARY_BIN_OUTOUT_DIRECTORY})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/lib)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/lib)

else()
    add_definitions(-std=c++11)

    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../Ipopt/include)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../clp/include)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../eigen)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../alglib/src)

    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../OpenBLAS/lib)
    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../Ipopt/lib)
    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../clp/lib)
    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../alglib/lib)

    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/lib)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/lib)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/lib)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/lib)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/bin)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/bin)

endif()

add_subdirectory(pfopt)
add_subdirectory(examples)
add_subdirectory(tests)

if(${TEST})
    if(MSVC)
        include_directories($ENV{GTEST_ROOT}/include)
        link_directories($ENV{GTEST_ROOT}/lib/$ENV{BUILD_TYPE}/MD)
    endif()
    add_subdirectory(tests)
endif()
