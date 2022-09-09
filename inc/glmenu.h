
#pragma once

#include <vector>
#include <memory>
#include "glmenuitem.h"

namespace mygl
{
    class Menu : public MenuItem
    {
    public:
        Menu(const std::string& label, const std::vector<MenuItem*>& menuItems);
        virtual ~Menu() noexcept;

        mygl::MenuItem* next();

        mygl::MenuItem* current();

        mygl::MenuItem* previous();

        void enter();

        bool back();

        mygl::Menu* topLevel();
        
        mygl::Menu* bottomLevel();

        std::vector<mygl::MenuItem*>::iterator begin();

        mygl::Menu* setIterator(std::vector<mygl::MenuItem*>::iterator it);

        mygl::MenuItem* itemAt(size_t index);

        virtual bool perform(mygl::Menu* menu) override;

        virtual std::string label() const override;

        void setExpanded(mygl::Menu* expanded) { _expanded = expanded; }
        mygl::Menu* expanded() const { return _expanded; }
        void setParent(mygl::Menu* parent) { _parent = parent; }
        mygl::Menu* parent() const { return _parent; }

    protected:
        mygl::Menu* _expanded{nullptr};
        mygl::Menu* _parent{nullptr};
        std::vector<MenuItem*> _menuItems;
        std::vector<MenuItem*>::iterator _menuIterator{_menuItems.begin()};
    };
}