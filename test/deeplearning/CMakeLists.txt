cmake_minimum_required(VERSION 3.22) 

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(testdeeplearning)

include_directories(
    ../../include
)

file(GLOB SOURCES
    testdeeplearning.cpp
)

add_executable(testdeeplearning ${SOURCES})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

install(TARGETS testdeeplearning DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)