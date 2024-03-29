
#pragma once

#include <string>
#include <functional>

namespace lithium
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

        virtual int perform(lithium::Menu* parent)
        {
            if(_callback)
            {
                _callback(this);
                return _actionId;
            }
            return _actionId;
        }

        bool enabled() const
        {
            return _enabled;
        }

        lithium::MenuItem* setEnabled(bool enabled)
        {
            _enabled = enabled;
            return this;
        }

        lithium::MenuItem* setCallback(std::function<void(lithium::MenuItem*)> callback)
        {
            _callback = callback;
            return this;
        }

        lithium::MenuItem* setActionId(int actionId)
        {
            _actionId = actionId;
            return this;
        }

        int actionId() const
        {
            return _actionId;
        }

        lithium::MenuItem* setId(int id)
        {
            _id = id;
            return this;
        }

        int id() const {return _id;}

    protected:
        std::string _label;
        bool _enabled{true};
        std::function<void(lithium::MenuItem*)> _callback;
        int _id{-1};
        int _actionId{0};
    };
}