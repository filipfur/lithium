
#pragma once

#include <vector>
#include <memory>
#include "glmenuitem.h"

namespace lithium
{
    class Menu : public MenuItem
    {
    public:
        Menu(const std::string& label, const std::vector<MenuItem*>& menuItems);
        virtual ~Menu() noexcept;

        lithium::MenuItem* next();

        lithium::MenuItem* current();

        lithium::MenuItem* previous();

        void enter();

        bool back();

        lithium::Menu* topLevel();
        
        lithium::Menu* bottomLevel();

        std::vector<lithium::MenuItem*>::iterator begin();

        lithium::Menu* setIterator(std::vector<lithium::MenuItem*>::iterator it);

        lithium::MenuItem* itemAt(size_t index);

        virtual bool perform(lithium::Menu* menu) override;

        virtual std::string label() const override;

        void setExpanded(lithium::Menu* expanded) { _expanded = expanded; }
        lithium::Menu* expanded() const { return _expanded; }
        void setParent(lithium::Menu* parent) { _parent = parent; }
        lithium::Menu* parent() const { return _parent; }

    protected:
        lithium::Menu* _expanded{nullptr};
        lithium::Menu* _parent{nullptr};
        std::vector<MenuItem*> _menuItems;
        std::vector<MenuItem*>::iterator _menuIterator{_menuItems.begin()};
    };
}