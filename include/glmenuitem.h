
#pragma once

#include <string>
#include <functional>

namespace mygl
{
    class Menu;

    class MenuItem
    {
    public:
        MenuItem(const std::string& label);
        virtual ~MenuItem() noexcept;
        
        virtual std::string label() const
        {
            return _label;
        }

        virtual bool perform(mygl::Menu* parent)
        {
            if(_callback)
            {
                _callback(this);
                return true;
            }
            return false;
        }

        bool enabled() const
        {
            return _enabled;
        }

        mygl::MenuItem* setEnabled(bool enabled)
        {
            _enabled = enabled;
            return this;
        }

        mygl::MenuItem* setCallback(std::function<void(mygl::MenuItem*)> callback)
        {
            _callback = callback;
            return this;
        }

        mygl::MenuItem* setId(int id)
        {
            _id = id;
            return this;
        }

        int id() const {return _id;}

    protected:
        std::string _label;
        bool _enabled{true};
        std::function<void(mygl::MenuItem*)> _callback;
        int _id{-1};
    };
}