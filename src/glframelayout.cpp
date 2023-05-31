#include "glframelayout.h"

std::string toString(const lithium::FrameLayout::Mode& mode)
{
    switch(mode)
    {
    case lithium::FrameLayout::Mode::Fixed:
        return "fixed";
    case lithium::FrameLayout::Mode::Fill:
        return "fill";
    case lithium::FrameLayout::Mode::Wrap:
        return "wrap";
    default:
        return "unknown";
    }
}

std::string toString(const lithium::FrameLayout::Orientation& orientation)
{
    switch(orientation)
    {
    case lithium::FrameLayout::Orientation::Horizontal:
        return "horizontal";
    case lithium::FrameLayout::Orientation::Vertical:
        return "vertical";
    default:
        return "unknown";
    }
}

std::string toString(const lithium::FrameLayout::Alignment& alignment)
{
    switch(alignment)
    {
    case lithium::FrameLayout::Alignment::Left:
        return "left";
    case lithium::FrameLayout::Alignment::Right:
        return "right";
    case lithium::FrameLayout::Alignment::Top:
        return "top";
    case lithium::FrameLayout::Alignment::Bottom:
        return "bottom";
    case lithium::FrameLayout::Alignment::Center:
        return "center";
    default:
        return "unknown";
    }
}

std::ostream& lithium::operator<<(std::ostream& os, const lithium::FrameLayout& frameLayout)
{
    os << "[FrameLayout]" << std::endl;
    os << "  mode: " << toString(frameLayout._mode) << std::endl;
    os << "  orientation: " << toString(frameLayout._orientation) << std::endl;
    os << "  alignment: " << toString(frameLayout._alignment) << std::endl;
    os << "  margin: " << frameLayout._margin.x << ", " << frameLayout._margin.y << ", " << frameLayout._margin.z << ", " << frameLayout._margin.w << std::endl;
    os << "  padding: " << frameLayout._padding.x << ", " << frameLayout._padding.y << ", " << frameLayout._padding.z << ", " << frameLayout._padding.w << std::endl;
    if(frameLayout._mode == FrameLayout::Mode::Fixed)
    {
        os << "  position: " << frameLayout._position.x << ", " << frameLayout._position.y << std::endl;
    }
    os << (frameLayout._mode == FrameLayout::Mode::Fixed ? "  dimension: " : "  maxDimension: ") << frameLayout._dimension.x << ", " << frameLayout._dimension.y << std::endl;
    os << "  children: " << frameLayout._children.size() << std::endl;
    return os;
}