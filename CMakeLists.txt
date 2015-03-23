cmake_minimum_required(VERSION 2.8)
project(JSON)

if(UNIX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  if("${CMAKE_CXX_COMPILER}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcolor-diagnostics")
  endif()
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(JSON
  ${CMAKE_CURRENT_SOURCE_DIR}/include/json/json.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/source/json.cpp)

add_executable(UnitJSON
  ${CMAKE_CURRENT_SOURCE_DIR}/test/main.cpp)
