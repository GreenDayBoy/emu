cmake_minimum_required(VERSION 2.8)
project(eMU CXX)

# Compiler detection ----------------------------
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    if(GCC_VERSION VERSION_LESS 4.7)
        message(FATAL_ERROR "Sorry, GCC 4.7 or higher is required")
    endif()
endif()
# -----------------------------------------------

# Common variables --------------
set(eMU_base_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(eMU_tmp_DIR ${eMU_base_DIR}/tmp)

set(eMU_src_DIR ${eMU_base_DIR}/src)
set(eMU_core_DIR ${eMU_src_DIR}/core)

set(eMU_tst_DIR ${eMU_base_DIR}/tst)
set(eMU_ut_DIR ${eMU_tst_DIR}/ut)
set(eMU_mt_DIR ${eMU_tst_DIR}/mt)
set(eMU_asioStub_DIR ${eMU_tst_DIR}/asioStub)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${eMU_base_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${eMU_base_DIR}/bin)
# -------------------------------

# Build type --------------------
message(STATUS "Configuring build type ${CMAKE_BUILD_TYPE}")
# -------------------------------

# Includes ----------------------
include_directories(${eMU_src_DIR})
include_directories(${eMU_core_DIR})

include_directories(${eMU_tst_DIR})
#--------------------------------

# Libs --------------------------
link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
# -------------------------------

# core setup -----------------
file(GLOB_RECURSE core_SOURCES "${eMU_core_DIR}/*.cpp")

add_library(core SHARED ${core_SOURCES})

target_link_libraries(core glog boost_system)
add_dependencies(core asioStub)
# -------------------------------

# mt/ut tests setup ----------------
file(GLOB_RECURSE asioStub_SOURCES "${eMU_asioStub_DIR}/*.cpp")

# ut -------
file(GLOB_RECURSE ut_SOURCES "${eMU_ut_DIR}/*.cpp")
add_executable(ut EXCLUDE_FROM_ALL ${ut_SOURCES} ${core_SOURCES})
target_link_libraries(ut gmock gtest pthread glog boost_system)

# mt -------
file(GLOB_RECURSE mt_SOURCES "${eMU_mt_DIR}/*.cpp")
add_executable(mt EXCLUDE_FROM_ALL ${mt_SOURCES} ${core_SOURCES})
target_link_libraries(mt gtest pthread glog)
# -------------------------------

# Compiler flags ----------------
set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -pedantic")

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -DeMU_TARGET")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DeMU_TARGET")
set(CMAKE_CXX_FLAGS_UT "-g -O0 -DeMU_UT")
set(CMAKE_CXX_FLAGS_MT "-g -O0 -DeMU_MT")
# -------------------------------
