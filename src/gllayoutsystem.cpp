#include <algorithm>

#include "gllayoutsystem.h"

lithium::LayoutSystem::LayoutSystem(const lithium::json::Json& json) : _root{}
{
    parse(json, _root);
}

bool insensitiveCompare(const std::string& a, const std::string& b)
{
    if(a.length() != b.length())
    {
        return false;
    }
    for(size_t i = 0; i < a.length(); ++i)
    {
        if(std::tolower(a[i]) != std::tolower(b[i]))
        {
            return false;
        }
    }
    return true;
}

glm::vec4 parseMarginPadding(const lithium::json::Json& json)
{
    try
    {
        if(json.size() == 1)
        {
            return glm::vec4{json[0].get<float>(), json[0].get<float>(), json[0].get<float>(), json[0].get<float>()};
        }
        else if(json.size() == 2)
        {
            return glm::vec4{json[0].get<float>(), json[1].get<float>(), json[0].get<float>(), json[1].get<float>()};
        }
        else if(json.size() == 4)
        {
            return glm::vec4{json[0].get<float>(), json[1].get<float>(), json[2].get<float>(), json[3].get<float>()};
        }
        else
        {
            throw std::runtime_error("invalid size of margin/padding");
        }
    }
    catch(...)
    {
        throw std::runtime_error("invalid margin/padding");
    }
}

void lithium::LayoutSystem::parse(const lithium::json::Json& json, FrameLayout& frameLayout)
{
    auto mode{FrameLayout::Mode::Fixed};
    auto orientation{FrameLayout::Orientation::Vertical};
    auto alignment{FrameLayout::Alignment::Center};
    glm::vec4 margin{0.0f};
    glm::vec4 padding{0.0f};
    glm::vec2 dimension{0.0f};
    glm::vec2 position{0.0f};
    if(json.contains("mode"))
    {
        if(insensitiveCompare(json["mode"], "fixed"))
        {
            mode = FrameLayout::Mode::Fixed;
        }
        else if(insensitiveCompare(json["mode"], "fill"))
        {
            mode = FrameLayout::Mode::Fill;
        }
        else if(insensitiveCompare(json["mode"], "wrap"))
        {
            mode = FrameLayout::Mode::Wrap;
        }
        else
        {
            throw std::runtime_error("invalid mode");
        }
    }
    if(json.contains("orientation"))
    {
        if(insensitiveCompare(json["orientation"], "horizontal"))
        {
            orientation = FrameLayout::Orientation::Horizontal;
        }
        else if(insensitiveCompare(json["orientation"], "vertical"))
        {
            orientation = FrameLayout::Orientation::Vertical;
        }
        else
        {
            throw std::runtime_error("invalid orientation");
        }
    }
    if(json.contains("alignment"))
    {
        if(insensitiveCompare(json["alignment"], "left"))
        {
            alignment = FrameLayout::Alignment::Left;
        }
        else if(insensitiveCompare(json["alignment"], "right"))
        {
            alignment = FrameLayout::Alignment::Right;
        }
        else if(insensitiveCompare(json["alignment"], "top"))
        {
            alignment = FrameLayout::Alignment::Top;
        }
        else if(insensitiveCompare(json["alignment"], "bottom"))
        {
            alignment = FrameLayout::Alignment::Bottom;
        }
        else if(insensitiveCompare(json["alignment"], "center"))
        {
            alignment = FrameLayout::Alignment::Center;
        }
        else
        {
            throw std::runtime_error("invalid alignment");
        }
    }
    if(json.contains("margin"))
    {
        margin = parseMarginPadding(json["margin"]);
    }
    if(json.contains("padding"))
    {
        padding = parseMarginPadding(json["padding"]);
    }
    if(json.contains("dimension"))
    {
        try
        {
            if(json["dimension"].isArray())
            {
                dimension = glm::vec2{json["dimension"][0].get<float>(), json["dimension"][1].get<float>()};
            }
            else
            {
                dimension = glm::vec2{json["dimension"].get<float>(), json["dimension"].get<float>()};
            }
        }
        catch(...)
        {
            throw std::runtime_error("invalid dimension");
        }
    }
    if(json.contains("position"))
    {
        if(mode != FrameLayout::Mode::Fixed)
        {
            throw std::runtime_error("position is ill-formed for non-fixed layouts");
        }
        try
        {
            if(json["position"].isArray())
            {
                position = glm::vec2{json["position"][0].get<float>(), json["position"][1].get<float>()};
            }
            else
            {
                position = glm::vec2{json["position"].get<float>(), json["position"].get<float>()};
            }
        }
        catch(...)
        {
            throw std::runtime_error("invalid position");
        }
    }
    frameLayout._mode = mode;
    frameLayout._orientation = orientation;
    frameLayout._alignment = alignment;
    frameLayout._margin = margin;
    frameLayout._padding = padding;
    frameLayout._dimension = dimension;
    frameLayout._position = position;
    if(json.contains("children"))
    {
        if(!json["children"].isArray())
        {
            throw std::runtime_error("children must be an array");
        }
        for(const auto& child : json["children"])
        {
            FrameLayout childFrameLayout{};
            parse(child, childFrameLayout);
            frameLayout._children.push_back(childFrameLayout);
        }
    }
    

    std::cout << frameLayout << std::endl;
}