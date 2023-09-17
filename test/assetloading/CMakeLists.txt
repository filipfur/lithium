cmake_minimum_required(VERSION 3.22) 

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(testassetloading)

include_directories(
    ../../dep/glm
    ../../include/lithium/utility
)

file(GLOB SOURCES
    ../../src/glfilesystem.cpp
    testassetloading.cpp
)

add_executable(testassetloading ${SOURCES})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

#install(FILES test.json DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)
#install(TARGETS testjson DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)