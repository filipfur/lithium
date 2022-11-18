
#pragma once

#include <functional>
#include "glmenuitem.h"

namespace lithium
{
    template <class T>
    class MenuOption : public MenuItem
    {
    public:
        MenuOption(const std::string& label, T* dataPtr, const std::function<void(T*)>& toggleCallback) : MenuItem{label}, _dataPtr{dataPtr}, _toggleCallback{toggleCallback}
        {

        }
        
        virtual ~MenuOption() noexcept
        {

        }

    private:
        const std::function<void(T*)> _toggleCallback;
        T* _dataPtr{nullptr};
    };
}