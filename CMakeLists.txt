cmake_minimum_required(VERSION 3.9)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(fogl VERSION 1.0.0 DESCRIPTION "Lightweight C++/OpenGL Game Engine.")

add_library(${PROJECT_NAME} STATIC 
    dep/glad/src/glad.c 
    src/debugostream.cpp
    src/glanimatedobject.cpp
    src/glcamera.cpp
    src/glcubemap.cpp
    src/glframebuffer.cpp
    src/glinput.cpp
    src/gllight.cpp
    src/glmenu.cpp
    src/glmenuitem.cpp
    src/globject.cpp
    src/glorthographiccamera.cpp
    src/glperspectivecamera.cpp
    src/glrenderbuffer.cpp
    src/glshaderprogram.cpp
    src/glsoundengine.cpp
    src/gltext.cpp
    src/gltexture.cpp
    src/gltimemeasure.cpp
    src/gltriangle.cpp
    src/stb_image.cpp
    src/taskscheduler.cpp
)

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION 1)

target_include_directories(${PROJECT_NAME} PRIVATE include
    dep/glad/include
    dep/glm
    dep/stb
    dep/json/include 
)

set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER include/*.h)

include(GNUInstallDirs)

target_link_libraries(${PROJECT_NAME} glfw opengl32)
add_subdirectory(dep/glfw-3.3.7)
add_subdirectory(dep/json)

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
