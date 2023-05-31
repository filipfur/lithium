#pragma once

#include <vector>
#include <ostream>
#include <glm/glm.hpp>

namespace lithium
{
    class IFrameLayout
    {
    public:
        virtual ~IFrameLayout() noexcept = default;

        virtual void onLayoutChanged(const glm::vec2& position, const glm::vec2& dimension) = 0;
    };

    class FrameLayout
    {
    public:
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

        /*
            Fixed:  The layout will be size dimension and at position. Fixed layouts can only coexist with other Fixed layouts.
                    Margin is not used.
            
            Fill:   The layout will try to expand to fill the parent layout but might share this space with other Fill or Wrap.
                    If a non-zero dimension is set, the layout wont expand beyond that dimension.
                    Position is not used.
            
            Wrap:   The layout will expand to contain the children layouts.
                    If a non-zero dimension is set, the layout wont expand beyond that dimension.
                    Position is not used.
        */
        enum class Mode
        {
            Fixed,
            Fill,
            Wrap
        };

        friend class LayoutSystem;

        friend std::ostream& operator<<(std::ostream& os, const FrameLayout& frameLayout);

    private:
        Mode _mode{Mode::Fixed};
        Orientation _orientation{Orientation::Vertical};
        Alignment _alignment{Alignment::Center};
        glm::vec4 _margin{0.0f};
        glm::vec4 _padding{0.0f};
        glm::vec2 _dimension{0.0f};
        glm::vec2 _position{0.0f};
        IFrameLayout* _iFrameLayout{nullptr};
        FrameLayout* _parent{nullptr};
        std::vector<FrameLayout> _children;
    };
}