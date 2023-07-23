cmake_minimum_required(VERSION 3.9)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(lithium VERSION 1.0.0 DESCRIPTION "Lightweight C++/OpenGL game engine.")

set(GLAD_DIR dep/glad_opengl_4_6)

if (apple)
    set(GLAD_DIR dep/glad_opengl_3_3)
endif()

add_library(lithium STATIC 
    ${GLAD_DIR}/src/glad.c
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
    src/glaabb.cpp
    src/glsphere.cpp
    src/glscene.cpp
    src/dlmatrix.cpp
    src/dlvector.cpp
    src/glplane.cpp
    src/glcube.cpp
    src/glplot.cpp
    src/glframe.cpp
    src/glcanvas.cpp
    src/glframerenderer.cpp
    src/gljson.cpp
    src/gllayoutsystem.cpp
    src/glframelayout.cpp
    src/glimagetexture.cpp
)

set_target_properties(lithium PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(lithium PROPERTIES SOVERSION 1)

target_include_directories(lithium PUBLIC
    include/lithium/application
    include/lithium/core
    include/lithium/render
    include/lithium/utility
    include/lithium/collision
    include/lithium/deeplearning
    include/lithium/ecs
    include/lithium/ui
    include/lithium/shape
    include/lithium
    ${GLAD_DIR}/include
    dep/glm
    dep/tinyobjloader
    dep/miniaudio
)

set_target_properties(lithium PROPERTIES PUBLIC_HEADER include/*.h)

include(GNUInstallDirs)

if (WIN32) 
    target_link_libraries(lithium glfw opengl32)
elseif (apple)
    target_link_libraries(lithium glfw
        "-framework OpenGL")
else ()
    target_link_libraries(lithium glfw)
endif ()

add_subdirectory(dep/glfw)

add_subdirectory(test/deeplearning)
add_subdirectory(test/vector)
add_subdirectory(test/geometry)
add_subdirectory(test/json)
add_subdirectory(test/ui)

install(TARGETS lithium
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
