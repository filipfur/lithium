#include "glmenu.h"

mygl::Menu::Menu(const std::string& label, const std::vector<MenuItem*>& menuItems) : MenuItem{label}, _menuItems{menuItems}
{

}

mygl::Menu::~Menu() noexcept
{

}

mygl::MenuItem* mygl::Menu::next()
{
    if(_expanded)
    {
        return _expanded->next();
    }
    if(!_menuItems.empty())
    {
        ++_menuIterator;
        if(_menuIterator == _menuItems.end())
        {
            _menuIterator = _menuItems.begin();
        }
        if(!current()->enabled())
        {
            next();
        }
        return *_menuIterator;
    }
    return nullptr;
}

std::vector<mygl::MenuItem*>::iterator mygl::Menu::begin()
{
    return _menuItems.begin();
}

mygl::Menu* mygl::Menu::setIterator(std::vector<mygl::MenuItem*>::iterator it)
{
    _menuIterator = it;
    return this;
}

mygl::MenuItem* mygl::Menu::current()
{
    if(_expanded)
    {
        return _expanded->current();
    }
    if(_menuIterator != _menuItems.end())
    {
        return *_menuIterator;
    }
    return nullptr;
}


mygl::MenuItem* mygl::Menu::previous()
{
    if(_expanded)
    {
        return _expanded->previous();
    }
    if(!_menuItems.empty())
    {
        if(_menuIterator == _menuItems.begin())
        {
            _menuIterator = _menuItems.end() - 1;
        }
        else
        {
            --_menuIterator;
        }
        if(!current()->enabled())
        {
            previous();
        }
        return *_menuIterator;
    }
    return nullptr;
}

void mygl::Menu::enter()
{
    if(_expanded)
    {
        _expanded->enter();
    }
    else
    {
        if(_menuIterator != _menuItems.end())
        {
            if(!current()->perform(this))
            {
                parent()->setExpanded(nullptr);
            }
        }
    }
}

mygl::Menu* mygl::Menu::bottomLevel()
{
    if(_expanded)
    {
        return _expanded->bottomLevel();
    }
    return this;
}

mygl::Menu* mygl::Menu::topLevel()
{
    if(_parent)
    {
        return _parent->topLevel();
    }
    return this;
}

bool mygl::Menu::back()
{
    mygl::Menu* menu = bottomLevel();
    menu = menu->parent();
    if(menu)
    {
        menu->setExpanded(nullptr);
        return true;
    }
    return false;
}

mygl::MenuItem* mygl::Menu::itemAt(size_t index)
{
    if(_expanded)
    {
        return _expanded->itemAt(index);
    }
    if(_menuItems.size() <= index || index < 0)
    {
        return nullptr;
    }
    return _menuItems.at(index);
}

std::string mygl::Menu::label() const
{
    if(_expanded)
    {
        return _expanded->label();
    }
    return mygl::MenuItem::label();
}

bool mygl::Menu::perform(mygl::Menu* parent)
{
    setParent(parent);
    parent->setExpanded(this);
    return true;
}