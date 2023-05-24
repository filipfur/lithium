cmake_minimum_required(VERSION 3.22) 

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(testgeometry)

include_directories(
    ../../dep/glm
    ../../include/lithium/collision
)

file(GLOB SOURCES
    ../../src/glaabb.cpp
    ../../src/glsphere.cpp
    testgeom.cpp
)

add_executable(testgeometry ${SOURCES})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

install(TARGETS testgeometry DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)