#pragma once

#include <glm/glm.hpp>

namespace lithium
{
    struct Layout
    {
        enum class Orientation
        {
            Horizontal,
            Vertical
        };

        enum class Alignment
        {
            Left,
            Right,
            Top,
            Bottom,
            Center
        };

        enum class Mode
        {
            Fixed,
            Fill,
            Wrap
        };

        Orientation orientation{Orientation::Horizontal};
        Alignment alignment{Alignment::Left};
        Mode mode{Mode::Fixed};
        glm::vec2 margin{0.0f};
        glm::vec2 padding{0.0f};
        glm::vec2 dimension{0.0f};
        glm::vec2 position{0.0f};
    };
}