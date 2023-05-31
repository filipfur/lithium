cmake_minimum_required(VERSION 3.22) 

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(testui)

include_directories(
    ../../dep/glm
    ../../include/lithium/ui
    ../../include/lithium/utility
)

file(GLOB SOURCES
    ../../src/gljson.cpp
    ../../src/gllayoutsystem.cpp
    ../../src/glframelayout.cpp
    testui.cpp
)

add_executable(testui ${SOURCES})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

install(TARGETS testui DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)