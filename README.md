# lithium

Lithium is a lightweight game engine written in C++/OpenGL. The engine is designed to support source code oriented project as opposed to visual game development in an integrated environment. The engine is prefferably built with CMake.

![logotype](Kraxbox_logo_lithium_metal_2ff2069c-b84a-426c-bf92-e9831105a5df.png)

## Creating an application


## The basics of rendering
The basic flow of creating a renderable object and rendering it looks like this.
```c++
#include "globject.h"
#include "glshaderprogram.h"
#include "gltinyobjloader.h"

lithium::Mesh* mesh = lithium::tinyobjloader_load("assets/block.obj", lithium::Mesh::State::POS_NORMAL_UV);
lithium::ShaderProgram* shaderProgram = new lithium::ShaderProgram("assets/shader.vert", "assets/shader.frag");
lithium::ImageTexture* diffuseTexture = lithium::ImageTexture::load("assets/meme.png", GL_SRGB, GL_RGB);
lithium::Object* object = new lithium::Object(mesh, diffuseTexture);

const float dt{0.01f};
while(1<3)
{
    object->update(dt);
    object->shade(shaderProgram);
    object->draw();
    sleep(dt);
}
```

## Design
Lithium is written in object-oriented C++. It's separated into several class packages which are all described here. The main purposes for wrapping the OpenGL API in classes is to increase the level of handholding and decrease bloated and repetetive code.
### Core
The ```core``` package contains classes that are low-level and tighly coupled to the OpenGL graphics rendering API. Classes include:
* Element
* VertexArray
* Buffer
* UniformBufferObject
* Attribute Pointer
* FrameBuffer
* RenderBuffer
* Texture, ImageTexture
* CubeMap
* Shader
* ShaderProgram
* Updateable
### Render
The ```render``` package contains classes that uses the primitive classes in the ```core``` package to create renderable entitities. It also has functions for loading object files and skeletal animated models. Currently supported file formats are Wavefront (```.obj```) and Collada (```.dae```). Classes include:
* Mesh
* Camera, OrthographicCamera, PerspectiveCamera (**TODO**)
* SimpleCamera
* Object, AnimatedObject, InstancedObject
* Animation, Animator, Bone, ... (**TODO**)
* ParticleSystem
* Fader
* Model
* ModelLoader
* TinyObjectLoader
* Text
### Application
The ```application``` package contains classes that holds basic application functions like creating a window using GLFW, processing window input, playing sound fx and creating menu systems. This will give you a flying start in creating a lithium application. Classes include:
* Application
* Input
* Menu
* MenuItem
* MenuOption
* RenderPipeline
### Utility
The ```utility``` package contains utility functions that are not directly related to graphics rendering. It has a task scheduler for scheduling delayed tasks, a time measuring class used for tracing function calls, and a debug output stream. Classes include:
* DebugOstream
* Task, PeriodicTask, TurnTask
* TaskScheduler
* TimeMeasure