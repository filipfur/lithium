#pragma once

#include <vector>
#include <functional>
#include <ostream>
#include <glm/glm.hpp>
#include <string>
#include <set>

namespace lithium
{
    class FrameLayout;
    class IFrameLayout
    {
    public:
        virtual ~IFrameLayout() noexcept = default;

        virtual void onLayoutChanged(FrameLayout* frameLayout) = 0;

        virtual void onLayoutUpdated(const glm::vec2& position, const glm::vec2& dimension) = 0;
    };

    class FrameLayout
    {
    public:
        enum class Orientation
        {
            Horizontal = 0,
            Vertical = 1
        };

        enum class Alignment
        {
            Top,
            Left,
            Bottom,
            Right,
            Center
        };

        /*
            Absolute: The layout will be size dimension and at position. Absolute layouts can only coexist with other Absolute layouts.
                      Margin is not used.
            
            Fixed:    The layout will try to be size dimension. Margin is used to position the layout relative to the parent layout.
                      Fixed layout must have a dimension set.
                      Position is not used.

            Fill:     The layout will try to expand to fill the parent layout but might share this space with other Fill or Wrap.
                      If a non-zero dimension is set, the layout wont expand beyond that dimension.
                      Position is not used.
            
            Wrap:     The layout will expand to contain the children layouts.
                      If a non-zero dimension is set, the layout wont expand beyond that dimension.
                      Position is not used.
        */
        enum class Mode
        {
            Absolute,
            Fixed,
            Fill,
            Wrap
        };

        friend class LayoutSystem;

        FrameLayout()
        {

        }

        FrameLayout(const std::string& id, Mode mode = Mode::Absolute, Orientation orientation = Orientation::Vertical, Alignment alignment = Alignment::Center, const glm::vec4& margin = glm::vec4(0.0f), const glm::vec4& padding = glm::vec4(0.0f), const glm::vec2& dimension = glm::vec2(0.0f), const glm::vec2& position = glm::vec2(0.0f)) :
            _id(id),
            _mode(mode),
            _orientation(orientation),
            _alignment(alignment),
            _margin(margin),
            _padding(padding),
            _dimension(dimension),
            _position(position)
        {
        }

        virtual ~FrameLayout() noexcept
        {
            _children.clear();
            _parent = nullptr;
        }

        FrameLayout(const FrameLayout& other) :
            _id(other._id),
            _mode(other._mode),
            _orientation(other._orientation),
            _alignment(other._alignment),
            _margin(other._margin),
            _padding(other._padding),
            _dimension(other._dimension),
            _position(other._position),
            _actualDimension(other._actualDimension),
            _actualPosition(other._actualPosition),
            _parent(other._parent),
            _iFrameLayout(other._iFrameLayout),
            _children(other._children)
        {
        }

        FrameLayout(FrameLayout&& other) noexcept :
            _id(std::move(other._id)),
            _mode(other._mode),
            _orientation(other._orientation),
            _alignment(other._alignment),
            _margin(other._margin),
            _padding(other._padding),
            _dimension(other._dimension),
            _position(other._position),
            _actualDimension(other._actualDimension),
            _actualPosition(other._actualPosition),
            _parent(other._parent),
            _iFrameLayout(other._iFrameLayout),
            _children(std::move(other._children))
        {
        }

        FrameLayout& operator=(const FrameLayout& other)
        {
            _id = other._id;
            _mode = other._mode;
            _orientation = other._orientation;
            _alignment = other._alignment;
            _margin = other._margin;
            _padding = other._padding;
            _dimension = other._dimension;
            _position = other._position;
            _actualDimension = other._actualDimension;
            _actualPosition = other._actualPosition;
            _parent = other._parent;
            _iFrameLayout = other._iFrameLayout;
            _children = other._children;
            return *this;
        }

        FrameLayout& operator=(FrameLayout&& other) noexcept
        {
            _id = std::move(other._id);
            _mode = other._mode;
            _orientation = other._orientation;
            _alignment = other._alignment;
            _margin = other._margin;
            _padding = other._padding;
            _dimension = other._dimension;
            _position = other._position;
            _actualDimension = other._actualDimension;
            _actualPosition = other._actualPosition;
            _parent = other._parent;
            _iFrameLayout = other._iFrameLayout;
            _children = std::move(other._children);
            return *this;
        }

        friend std::ostream& operator<<(std::ostream& os, const FrameLayout& frameLayout);

        void attach(IFrameLayout* iFrameLayout)
        {
            _iFrameLayout = iFrameLayout;
        }

        std::string id() const
        {
            return _id;
        }
        
        Mode mode() const
        {
            return _mode;
        }

        Orientation orientation() const
        {
            return _orientation;
        }

        Alignment alignment() const
        {
            return _alignment;
        }

        glm::vec4 margin() const
        {
            return _margin;
        }

        glm::vec4 padding() const
        {
            return _padding;
        }

        glm::vec2 dimension() const
        {
            return _dimension;
        }

        glm::vec2 position() const
        {
            return _position;
        }

        glm::vec2 actualDimension() const
        {
            return _actualDimension;
        }

        glm::vec2 actualPosition() const
        {
            return _actualPosition;
        }

        FrameLayout* appendChild(const FrameLayout& child)
        {
            _children.push_back(child);
            FrameLayout* ptr = &_children.back();
            ptr->_parent = this;
            if(ptr->_id.length() == 0)
            {
                ptr->_id = _id + '.' + std::to_string(_children.size());
            }
            for(auto& child : _children)
            {
                if(child._iFrameLayout)
                {
                    child._iFrameLayout->onLayoutChanged(&child);
                }
            }
            return ptr;
        }

        lithium::FrameLayout* setMode(Mode mode)
        {
            _mode = mode;
            return this;
        }

        lithium::FrameLayout* setOrientation(Orientation orientation)
        {
            _orientation = orientation;
            return this;
        }

        size_t count() const
        {
            return _children.size();
        }

        lithium::FrameLayout* setAlignment(Alignment alignment)
        {
            _alignment = alignment;
            return this;
        }

        lithium::FrameLayout* setMargin(const glm::vec4& margin)
        {
            _margin = margin;
            return this;
        }

        lithium::FrameLayout* setPadding(const glm::vec4& padding)
        {
            _padding = padding;
            return this;
        }

        lithium::FrameLayout* setDimension(const glm::vec2& dimension)
        {
            _dimension = dimension;
            return this;
        }

        lithium::FrameLayout* setPosition(const glm::vec2& position)
        {
            _position = position;
            return this;
        }

        void forEachChild(std::function<void(FrameLayout&)> callback)
        {
            for(auto& child : _children)
            {
                callback(child);
            }
        }

        FrameLayout* parent() const
        {
            return _parent;
        }

        IFrameLayout* iFrameLayout() const
        {
            return _iFrameLayout;
        }

        lithium::FrameLayout* clone() const
        {
            FrameLayout* ret{nullptr};
            FrameLayout* parent{nullptr};
            if(_parent == nullptr)
            {
                throw std::runtime_error("Cannot clone root layout");
            }
            else {
                FrameLayout clone{};
                parent = _parent;
                clone.clone(*this, _parent);
                std::set<lithium::FrameLayout*> stack{};
                stack.emplace(&clone);
                while(!stack.empty())
                {
                    FrameLayout* layout = *stack.begin();
                    stack.erase(stack.begin());
                    for(auto& child : layout->_children)
                    {
                        child._parent = layout;
                        stack.insert(&child);
                    }
                }
                for(auto& child : parent->_children)
                {
                    if(child._iFrameLayout)
                    {
                        child._iFrameLayout->onLayoutChanged(&child);
                    }
                }
                ret = &parent->_children.back();
            }
            return ret;
        }

        void clone(const FrameLayout& other, FrameLayout* parent)
        {
            _id = parent->_id + '.' + std::to_string(parent->_children.size());
            _mode = other._mode;
            _orientation = other._orientation;
            _alignment = other._alignment;
            _margin = other._margin;
            _padding = other._padding;
            _dimension = other._dimension;
            _position = other._position;
            _parent = parent;
            for(auto& child : other._children)
            {
                lithium::FrameLayout clone{};
                clone.clone(child, this);
            }
            parent->_children.push_back(*this);
        }

    private:
        std::string _id;
        Mode _mode{Mode::Absolute};
        Orientation _orientation{Orientation::Vertical};
        Alignment _alignment{Alignment::Center};
        glm::vec4 _margin{0.0f};
        glm::vec4 _padding{0.0f};
        glm::vec2 _dimension{0.0f};
        glm::vec2 _position{0.0f};
        glm::vec2 _actualDimension{0.0f};
        glm::vec2 _actualPosition{0.0f};
        IFrameLayout* _iFrameLayout{nullptr};
        FrameLayout* _parent{nullptr};
        std::vector<FrameLayout> _children;
    };
}