#pragma once

#include "glmesh.h"

namespace lithium
{
    lithium::Mesh* Plane2D(const glm::vec2& scale=glm::vec2{1.0f}, const glm::vec2& uvScale = glm::vec2{1.0f});
    lithium::Mesh* Plane3D();
}