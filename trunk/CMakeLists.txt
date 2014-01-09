cmake_minimum_required(VERSION 2.8.10)
project(eMU CXX)

# Common variables --------------
set(eMU_base_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(eMU_sources_DIR ${eMU_base_DIR}/src)
set(eMU_include_DIR ${eMU_base_DIR}/include)
set(eMU_tst_DIR ${eMU_base_DIR}/tst)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${eMU_base_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${eMU_base_DIR}/bin)
# -------------------------------

# Build type --------------------
message(STATUS "Configuring build type ${CMAKE_BUILD_TYPE}")
# -------------------------------

# Includes ----------------------
include_directories(${eMU_include_DIR})
include_directories(${QT_ROOT}/include)
#--------------------------------

# Libs --------------------------
link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY} ${QT_ROOT}/lib)

set(eMU_shared_libraries boost_system boost_thread boost_log gflags)
if(WIN32)
    set(eMU_shared_libraries ${eMU_shared_libraries} ws2_32 mswsock)
else()
    set(eMU_shared_libraries ${eMU_shared_libraries} pthread)
endif()
# -------------------------------

# core setup -----------------
set(eMU_core_sources_DIR ${eMU_sources_DIR}/core)
set(eMU_core_headers_DIR ${eMU_include_DIR}/core)

file(GLOB_RECURSE eMU_core_FILES ${eMU_core_sources_DIR}/*.cpp ${eMU_core_headers_DIR}/*.hpp)

add_library(core SHARED ${eMU_core_FILES})
target_link_libraries(core ${eMU_shared_libraries})
# -------------------------------

# streaming setup -----------------
set(eMU_streaming_sources_DIR ${eMU_sources_DIR}/streaming)
set(eMU_streaming_headers_DIR ${eMU_include_DIR}/streaming)
file(GLOB_RECURSE eMU_streaming_FILES ${eMU_streaming_sources_DIR}/*.cpp ${eMU_streaming_headers_DIR}/*.hpp)

add_library(streaming SHARED ${eMU_streaming_FILES})
target_link_libraries(streaming core)
add_dependencies(streaming core)
# -------------------------------

# protocols setup -----------------
set(eMU_protocols_sources_DIR ${eMU_sources_DIR}/protocols)
set(eMU_protocols_headers_DIR ${eMU_include_DIR}/protocols)
file(GLOB_RECURSE eMU_protocols_FILES ${eMU_protocols_sources_DIR}/*.cpp ${eMU_protocols_headers_DIR}/*.hpp)

add_library(protocols SHARED ${eMU_protocols_FILES})
target_link_libraries(protocols core streaming)
add_dependencies(protocols core streaming)
# -------------------------------

# dataserver setup ----------------
set(eMU_dataserver_sources_DIR ${eMU_sources_DIR}/dataserver)
set(eMU_dataserver_headers_DIR ${eMU_include_DIR}/dataserver)
file(GLOB_RECURSE eMU_dataserver_FILES ${eMU_dataserver_sources_DIR}/*.cpp ${eMU_dataserver_headers_DIR}/*.hpp)

add_executable(dataserver ${eMU_dataserver_FILES})
target_link_libraries(dataserver core streaming protocols mysqlclient ${eMU_shared_libraries})
add_dependencies(dataserver core streaming protocols)
# ------------------------------------

# loginserver setup ----------------
set(eMU_loginserver_sources_DIR ${eMU_sources_DIR}/loginserver)
set(eMU_loginserver_headers_DIR ${eMU_include_DIR}/loginserver)
file(GLOB_RECURSE eMU_loginserver_FILES ${eMU_loginserver_sources_DIR}/*.cpp ${eMU_loginserver_headers_DIR}/*.hpp)

add_executable(loginserver ${eMU_loginserver_FILES})
target_link_libraries(loginserver core streaming protocols ${eMU_shared_libraries})
add_dependencies(loginserver core streaming protocols)
# ------------------------------------

# gameserver setup ----------------
set(eMU_gameserver_sources_DIR ${eMU_sources_DIR}/gameserver)
set(eMU_gameserver_headers_DIR ${eMU_include_DIR}/gameserver)
file(GLOB_RECURSE eMU_gameserver_FILES ${eMU_gameserver_sources_DIR}/*.cpp ${eMU_gameserver_headers_DIR}/*.hpp)

add_executable(gameserver ${eMU_gameserver_FILES})
target_link_libraries(gameserver core streaming protocols ${eMU_shared_libraries})
add_dependencies(gameserver core streaming protocols)
# ------------------------------------

# analyzer setup -------------
set(eMU_analyzer_sources_DIR ${eMU_sources_DIR}/analyzer)
set(eMU_analyzer_headers_DIR ${eMU_include_DIR}/analyzer)
file(GLOB_RECURSE eMU_analyzer_FILES ${eMU_analyzer_sources_DIR}/*.cpp ${eMU_analyzer_headers_DIR}/*.hpp)

add_executable(analyzer EXCLUDE_FROM_ALL ${eMU_analyzer_FILES})
target_link_libraries(analyzer core protocols ${eMU_shared_libraries} Qt5Widgets Qt5Core Qt5Gui)
add_dependencies(analyzer core)
# ----------------------------

# ut -------------------------
set(eMU_ut_sources_DIR ${eMU_tst_DIR}/ut)
set(eMU_ut_headers_DIR ${eMU_include_DIR}/ut)
file(GLOB_RECURSE eMU_ut_FILES ${eMU_ut_sources_DIR}/*.cpp ${eMU_ut_headers_DIR}/*.hpp)
list(APPEND eMU_ut_FILES ${eMU_core_FILES} ${eMU_streaming_FILES} ${eMU_protocols_FILES} ${eMU_dataserver_FILES} ${eMU_loginserver_FILES} ${eMU_gameserver_FILES})
list(REMOVE_ITEM eMU_ut_FILES ${eMU_dataserver_sources_DIR}/main.cpp ${eMU_loginserver_sources_DIR}/main.cpp ${eMU_gameserver_sources_DIR}/main.cpp)

add_executable(ut EXCLUDE_FROM_ALL ${eMU_ut_FILES})
target_link_libraries(ut gmock gtest mysqlclient ${eMU_shared_libraries})
# ----------------------------

# mt ----------------------------
set(eMU_mt_sources_DIR ${eMU_tst_DIR}/mt)
set(eMU_mt_headers_DIR ${eMU_include_DIR}/mt)

file(GLOB_RECURSE eMU_mt_FILES ${eMU_mt_sources_DIR}/*.cpp ${eMU_mt_headers_DIR}/*.hpp)
list(APPEND eMU_mt_FILES ${eMU_core_FILES} ${eMU_streaming_FILES} ${eMU_protocols_FILES} ${eMU_dataserver_FILES} ${eMU_loginserver_FILES} ${eMU_gameserver_FILES})
list(REMOVE_ITEM eMU_mt_FILES ${eMU_dataserver_sources_DIR}/main.cpp ${eMU_loginserver_sources_DIR}/main.cpp ${eMU_gameserver_sources_DIR}/main.cpp)

add_executable(mt EXCLUDE_FROM_ALL ${eMU_mt_FILES})
target_link_libraries(mt gtest mysqlclient ${eMU_shared_libraries})
# -------------------------------

# Compiler flags ----------------
add_definitions(-DBOOST_ALL_DYN_LINK)

if(NOT WIN32)
    set(CMAKE_CXX_FLAGS "-fPIE")
else()
    add_definitions(-D_WIN32_WINNT=0x0501 -DWINVER=0x0501)
endif()

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -pedantic ${CMAKE_CXX_FLAGS}")

if(NOT COVERAGE)
    set(COVERAGE_FLAGS "")
else()
    message(STATUS "Build will be configured with coverage flags")
    set(COVERAGE_FLAGS "-fprofile-arcs -ftest-coverage")
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")
set(CMAKE_CXX_FLAGS_UT "-g -O0 -DeMU_UT ${COVERAGE_FLAGS}")
set(CMAKE_CXX_FLAGS_MT "-g -O0 -DeMU_MT ${COVERAGE_FLAGS}")
# -------------------------------
