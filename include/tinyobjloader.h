#pragma once

#include "glmesh.h"

mygl::Mesh* tinyobjloader_load(const char * inputfile, glm::vec2 uvScale=glm::vec2{1.0f, 1.0f}, bool print=false);