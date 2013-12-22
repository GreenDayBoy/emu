cmake_minimum_required(VERSION 2.8.10)
project(eMU CXX)

# Common variables --------------
set(eMU_base_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(eMU_src_DIR ${eMU_base_DIR}/src)
set(eMU_core_DIR ${eMU_src_DIR}/core)
set(eMU_protocol_DIR ${eMU_src_DIR}/protocol)
set(eMU_dataserver_DIR ${eMU_src_DIR}/dataserver)
set(eMU_loginserver_DIR ${eMU_src_DIR}/loginserver)
set(eMU_gameserver_DIR ${eMU_src_DIR}/gameserver)
set(eMU_analyzer_DIR ${eMU_src_DIR}/analyzer)

set(eMU_tst_DIR ${eMU_base_DIR}/tst)
set(eMU_ut_DIR ${eMU_tst_DIR}/ut)
set(eMU_mt_DIR ${eMU_tst_DIR}/mt)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${eMU_base_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${eMU_base_DIR}/bin)
# -------------------------------

# Build type --------------------
message(STATUS "Configuring build type ${CMAKE_BUILD_TYPE}")
# -------------------------------

# Includes ----------------------
include_directories(${eMU_src_DIR})
include_directories(${eMU_tst_DIR})
include_directories(${QT_ROOT}/include)
#--------------------------------

# Libs --------------------------
link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY} ${QT_ROOT}/lib)
# -------------------------------

# core setup -----------------
file(GLOB_RECURSE core_SOURCES "${eMU_core_DIR}/*[.cpp.hpp]")
add_library(core SHARED ${core_SOURCES})
target_link_libraries(core glog boost_system boost_thread)
# -------------------------------

# protocol setup -----------------
file(GLOB_RECURSE protocol_SOURCES "${eMU_protocol_DIR}/*[.cpp.hpp]")
add_library(protocol SHARED ${protocol_SOURCES})
add_dependencies(protocol core)
# -------------------------------

# dataserver setup ----------------
file(GLOB_RECURSE dataserver_SOURCES "${eMU_dataserver_DIR}/*[.cpp.hpp]")
add_executable(dataserver ${dataserver_SOURCES})
target_link_libraries(dataserver pthread core protocol mysqlclient gflags)
add_dependencies(dataserver core protocol)
# ------------------------------------

# loginserver setup ----------------
file(GLOB_RECURSE loginserver_SOURCES "${eMU_loginserver_DIR}/*[.cpp.hpp]")
add_executable(loginserver ${loginserver_SOURCES})
target_link_libraries(loginserver pthread core protocol gflags)
add_dependencies(loginserver core protocol)
# ------------------------------------

# gameserver setup ----------------
file(GLOB_RECURSE gameserver_SOURCES "${eMU_gameserver_DIR}/*[.cpp.hpp]")
add_executable(gameserver ${gameserver_SOURCES})
target_link_libraries(gameserver pthread core protocol gflags)
add_dependencies(gameserver core protocol)
# ------------------------------------

# mt/ut tests setup ----------------
# ut -------
file(GLOB_RECURSE ut_SOURCES "${eMU_ut_DIR}/*[.cpp.hpp]")
add_executable(ut EXCLUDE_FROM_ALL ${ut_SOURCES} ${core_SOURCES} ${dataserver_SOURCES} ${protocol_SOURCES} ${loginserver_SOURCES} ${gameserver_SOURCES})
target_link_libraries(ut gmock gtest pthread glog boost_system boost_thread mysqlclient)

# mt -------
file(GLOB_RECURSE mt_SOURCES "${eMU_mt_DIR}/*[.cpp.hpp]")
add_executable(mt EXCLUDE_FROM_ALL ${mt_SOURCES} ${core_SOURCES} ${dataserver_SOURCES} ${protocol_SOURCES} ${loginserver_SOURCES} ${gameserver_SOURCES})
target_link_libraries(mt gtest pthread glog boost_system boost_thread mysqlclient)
# -------------------------------

# analyzer setup -------------
file(GLOB_RECURSE analyzer_SOURCES "${eMU_analyzer_DIR}/*[.cpp.hpp]")
add_executable(analyzer EXCLUDE_FROM_ALL ${analyzer_SOURCES})
target_link_libraries(analyzer core protocol pthread Qt5Widgets Qt5Core Qt5Gui)
add_dependencies(analyzer core protocol)
# ----------------------------

# Compiler flags ----------------
set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -pedantic -fPIE")

if(NOT COVERAGE)
    set(COVERAGE_FLAGS "")
else()
    message(STATUS "Build will be configured with coverage flags")
    set(COVERAGE_FLAGS "-fprofile-arcs -ftest-coverage")
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -DeMU_TARGET")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DeMU_TARGET")
set(CMAKE_CXX_FLAGS_UT "-g -O0 -DeMU_UT ${COVERAGE_FLAGS}")
set(CMAKE_CXX_FLAGS_MT "-g -O0 -DeMU_MT ${COVERAGE_FLAGS}")
# -------------------------------
