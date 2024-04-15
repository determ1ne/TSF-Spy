cmake_minimum_required(VERSION 3.21.0)
project(tsfspy VERSION 0.1.0 LANGUAGES C CXX)

if (NOT MSVC)
  message(FATAL_ERROR "MSVC is required to build this project")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED 17)
add_subdirectory(vendor/fmt)
add_subdirectory(vendor/detours)

file(GLOB_RECURSE SOURCES "src/*.cc")
# add_executable(tsfspy ${SOURCES})
add_library(tsfspy SHARED ${SOURCES})
target_include_directories(tsfspy PRIVATE "include")

target_link_libraries(tsfspy fmt::fmt detours)
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /DEF:tsfspy.def")
