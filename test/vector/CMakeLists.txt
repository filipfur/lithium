cmake_minimum_required(VERSION 3.22) 

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(testvector)

include_directories(
    ../../include/lithium/deeplearning
    ../../dep/glm
)

file(GLOB SOURCES
    ../../src/dlvector.cpp
    ../../src/dlmatrix.cpp
    testvector.cpp
)

add_executable(testvector ${SOURCES})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

install(TARGETS testvector DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)