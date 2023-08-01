cmake_minimum_required(VERSION 3.22) 

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(testshader)

include_directories(
    ../../dep/glm
    ../../dep/glad_opengl_4_6/include
    ../../include/lithium/core
    ../../include/lithium/utility
)

file(GLOB SOURCES
    testshader.cpp
)

add_executable(testshader ${SOURCES})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

install(TARGETS testshader DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)