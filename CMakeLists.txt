cmake_minimum_required(VERSION 3.9)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(lithium VERSION 1.0.0 DESCRIPTION "Lightweight C++/OpenGL game engine.")

add_library(lithium STATIC 
    dep/glad_opengl_4_6/src/glad.c 
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
    src/glrenderbuffer.cpp
    src/glshaderprogram.cpp
    src/gltext.cpp
    src/gltimemeasure.cpp
    src/stb_image.cpp
    src/glperiodictask.cpp
    src/gltaskscheduler.cpp
    src/gltinyobjloader.cpp
    src/glnode.cpp
    src/glskinnedobject.cpp
    src/glrenderstage.cpp
    src/ecs.cpp
    src/utility.cpp
    src/glrenderable.cpp
    src/glfilewatch.cpp
)

set_target_properties(lithium PROPERTIES VERSION ${PROJECT_VERSION})

set_target_properties(lithium PROPERTIES SOVERSION 1)

target_include_directories(lithium PUBLIC
    include/lithium/application
    include/lithium/core
    include/lithium/render
    include/lithium/utility
    include/lithium/collision
    include/lithium/ecs
    include/lithium
    dep/glad_opengl_4_6/include
    dep/glm
    dep/json/include
    dep/tinyobjloader
    dep/miniaudio
)

set_target_properties(lithium PROPERTIES PUBLIC_HEADER include/*.h)

include(GNUInstallDirs)

if (WIN32) 
    target_link_libraries(lithium glfw opengl32)
else ()
    target_link_libraries(lithium glfw
        "-framework OpenGL")
endif ()

add_subdirectory(dep/glfw)
add_subdirectory(dep/json)

install(TARGETS lithium
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
