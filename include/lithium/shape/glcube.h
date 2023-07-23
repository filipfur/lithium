#pragma once

#include "glmesh.h"

namespace lithium
{
    Mesh* Cube(const glm::vec3& scale=glm::vec3{1.0f}, const glm::vec2& uvScale = glm::vec2{1.0f});
    Mesh* InvCube(float scale=1.0f);
}