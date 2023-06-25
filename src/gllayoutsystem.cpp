#include "gllayoutsystem.h"

#include <algorithm>
#include <iostream>
#include <unordered_set>
#include <set>
#include <functional>

namespace
{
    constexpr unsigned int TOP = 0;
    constexpr unsigned int RIGHT = 1;
    constexpr unsigned int BOTTOM = 2;
    constexpr unsigned int LEFT = 3;
    std::unordered_set<std::string> usedIds;
}

lithium::LayoutSystem::LayoutSystem() : _root{}
{

}

lithium::LayoutSystem::LayoutSystem(const lithium::json::Json& json) : LayoutSystem{}
{
    load(json);
}

void lithium::LayoutSystem::load(const lithium::json::Json& json)
{
    try
    {
        parse(json, _root);
        updateLayouts();
    }
    catch(const std::exception& e)
    {
        std::cerr << "failed to parse layout system: " << e.what();
    }
}

void lithium::LayoutSystem::updateLayouts()
{
    std::set<FrameLayout*> stack;

    std::function<void(FrameLayout&)> forEachChild = [&](FrameLayout& frameLayout)
    {
        for(auto& child : frameLayout._children)
        {
            child._parent = &frameLayout;
            if(child._children.empty())
            {
                stack.emplace(&child);
            }
            else
            {
                forEachChild(child);
            }
        }
    };

    forEachChild(_root);

    // Stack now contains all leaf nodes.

    while(!stack.empty())
    {
        FrameLayout* frameLayout = *stack.begin();
        stack.erase(stack.begin());
        if(frameLayout->_parent != nullptr)
        {
            stack.emplace(frameLayout->_parent);
        }

        if(frameLayout->_children.empty())
        {
            // Leaf node really nothing to do here.
            switch(frameLayout->_mode)
            {
                case FrameLayout::Mode::Absolute:
                    frameLayout->_actualPosition = frameLayout->_position;
                    // intentional fallthrough
                case FrameLayout::Mode::Fixed:
                case FrameLayout::Mode::Fill:
                    frameLayout->_actualDimension = frameLayout->_dimension;
                    break;
                case FrameLayout::Mode::Wrap:
                    throw std::runtime_error("wrap mode is ill-formed for leaf nodes");
                    break;
                default:
                    break;
            }
            continue;
        }
        float totalLength{0.0f}; // Length is the dimension of the orientation.
        float maxBreadth{0.0f}; // Breadth is the dimension of the non-orientation.
        int n = static_cast<int>(frameLayout->_orientation);
        int m = (n + 1) % 2;
        int numFills{0};
        for(auto& child : frameLayout->_children)
        {
            if(child._mode == FrameLayout::Mode::Absolute) // if so we can break
            {
                // Fixed mode really nothing to do here.
                break;
            }
            else if(child._mode == FrameLayout::Mode::Fill)
            {
                ++numFills;
            }

            float inc = child._dimension[n] + child._margin[m + TOP] + child._margin[m + BOTTOM];
            totalLength += inc;
            maxBreadth = std::max(maxBreadth, child._dimension[m] + child._margin[LEFT - m] + child._margin[RIGHT - m]);
        }
        switch(frameLayout->_mode)
        {
            case FrameLayout::Mode::Absolute:
                frameLayout->_actualPosition = frameLayout->_position;
                frameLayout->_actualDimension = frameLayout->_dimension;
                if(frameLayout->_dimension[n] == 0.0f)
                {
                    frameLayout->_actualDimension[n] = totalLength + frameLayout->_padding[m+TOP] + frameLayout->_padding[m+BOTTOM];
                }
                if(frameLayout->_dimension[m] == 0.0f)
                {
                    frameLayout->_actualDimension[m] = maxBreadth + frameLayout->_padding[LEFT - m] + frameLayout->_padding[RIGHT - m];
                }
                break;
            case FrameLayout::Mode::Fixed:
            case FrameLayout::Mode::Fill:
                frameLayout->_actualDimension = frameLayout->_dimension;
                break;
            case FrameLayout::Mode::Wrap:
                if(frameLayout->_dimension[n] == 0)
                {
                    frameLayout->_actualDimension[n] = totalLength;
                }
                else if(frameLayout->_dimension[n] < totalLength)
                {
                    throw std::runtime_error("max dimension for wrap mode is not supported yet");
                }
                else
                {
                    float remainder = frameLayout->_actualDimension[n] - totalLength;
                    float fill = remainder / numFills;
                    for(auto& child : frameLayout->_children)
                    {
                        if(child._mode == FrameLayout::Mode::Fill)
                        {
                            
                        }
                    }
                }
                if(frameLayout->_dimension[m] < maxBreadth)
                {
                    frameLayout->_dimension[m] = maxBreadth;
                }
                break;
            default:
                break;
        }
        float curPos{frameLayout->_actualDimension[n] * 0.5f - frameLayout->_padding[TOP]};
        for(auto& child : frameLayout->_children)
        {
            if(child._mode == FrameLayout::Mode::Absolute) // if so we can break
            {
                // Fixed mode really nothing to do here.
                break;
            }
            else
            {
                child._actualPosition[n] = curPos - child._actualDimension[n] * 0.5f - child._margin[m + TOP];
            }

            float inc = child._dimension[n] + child._margin[m + TOP] + child._margin[m + BOTTOM];
            curPos -= inc;
        }
    }
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
    if(json.contains("id"))
    {
        const std::string id = json["id"].get<std::string>();
        for(char c : id)
        {
            if(!std::isalpha(c) && c != '_' && c != '-')
            {
                throw std::runtime_error("invalid id, must be letter, dash, or underscore");
            }
        }
        if(usedIds.find(id) != usedIds.end())
        {
            throw std::runtime_error("layout id's must be unique");
        }
        usedIds.insert(id);
        frameLayout._id = id;
    }
    else if(frameLayout._parent)
    {
        frameLayout._id = frameLayout._parent->_id + '.' + std::to_string(frameLayout._parent->_children.size());
    }
    else
    {
        frameLayout._id = "0";
    }
    if(json.contains("mode"))
    {
        if(insensitiveCompare(json["mode"], "absolute"))
        {
            mode = FrameLayout::Mode::Absolute;
        }
        else if(insensitiveCompare(json["mode"], "fixed"))
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
        if(mode != FrameLayout::Mode::Absolute)
        {
            throw std::runtime_error("position is ill-formed for non-absolute layouts");
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
        bool hasAbsoluteLayout{false};
        for(const auto& child : json["children"])
        {
            FrameLayout childFrameLayout{};
            childFrameLayout._parent = &frameLayout;
            parse(child, childFrameLayout);
            if(hasAbsoluteLayout && childFrameLayout._mode != FrameLayout::Mode::Absolute)
            {
                throw std::runtime_error("non-absolute layouts cannot have siblings that are absolute");
            }
            hasAbsoluteLayout = childFrameLayout._mode == FrameLayout::Mode::Absolute;
            frameLayout._children.push_back(childFrameLayout);
        }
    }

    std::cout << frameLayout << std::endl;
}