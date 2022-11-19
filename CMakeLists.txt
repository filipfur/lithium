cmake_minimum_required(VERSION 3.9)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(lithium VERSION 1.0.0 DESCRIPTION "Lightweight C++/OpenGL game engine.")

add_library(${PROJECT_NAME} STATIC 
    dep/glad/src/glad.c 
    src/gldebugostream.cpp
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
    src/gltext.cpp
    src/gltimemeasure.cpp
    src/gltriangle.cpp
    src/stb_image.cpp
    src/glperiodictask.cpp
    src/gltaskscheduler.cpp
    src/gltinyobjloader.cpp
)

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION 1)

target_include_directories(${PROJECT_NAME} PUBLIC
    include/lithium/application
    include/lithium/core
    include/lithium/obsolete
    include/lithium/render
    include/lithium/utility
    dep/glad/include
    dep/glm
    dep/json/include
    dep/tinyobjloader
    dep/filewatch
    dep/assimp/include
    dep/miniaudio
)

set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER include/*.h)

include(GNUInstallDirs)

if (WIN32) 
    target_link_libraries(${PROJECT_NAME} glfw assimp opengl32)
else ()
    target_link_libraries(${PROJECT_NAME} glfw assimp
        "-framework OpenGL")
endif ()

add_subdirectory(dep/glfw)
add_subdirectory(dep/json)
add_subdirectory(dep/assimp)

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
