#include "glmenu.h"

lithium::Menu::Menu(const std::string& label, const std::vector<MenuItem*>& menuItems) : MenuItem{label}, _menuItems{menuItems}
{

}

lithium::Menu::~Menu() noexcept
{

}

lithium::MenuItem* lithium::Menu::next()
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

std::vector<lithium::MenuItem*>::iterator lithium::Menu::begin()
{
    return _menuItems.begin();
}

lithium::Menu* lithium::Menu::setIterator(std::vector<lithium::MenuItem*>::iterator it)
{
    _menuIterator = it;
    return this;
}

lithium::MenuItem* lithium::Menu::current()
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


lithium::MenuItem* lithium::Menu::previous()
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

int lithium::Menu::enter()
{
    int actionId{0};
    if(_expanded)
    {
        _expanded->enter();
    }
    else
    {
        if(_menuIterator != _menuItems.end())
        {
            actionId = current()->perform(this);
            if(actionId && parent())
            {
                parent()->setExpanded(nullptr);
            }
        }
    }
    return actionId;
}

lithium::Menu* lithium::Menu::bottomLevel()
{
    if(_expanded)
    {
        return _expanded->bottomLevel();
    }
    return this;
}

lithium::Menu* lithium::Menu::topLevel()
{
    if(_parent)
    {
        return _parent->topLevel();
    }
    return this;
}

bool lithium::Menu::back()
{
    lithium::Menu* menu = bottomLevel();
    menu = menu->parent();
    if(menu)
    {
        menu->setExpanded(nullptr);
        return true;
    }
    return false;
}

lithium::MenuItem* lithium::Menu::itemAt(size_t index)
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

std::string lithium::Menu::label() const
{
    if(_expanded)
    {
        return _expanded->label();
    }
    return lithium::MenuItem::label();
}

int lithium::Menu::perform(lithium::Menu* parent)
{
    setParent(parent);
    parent->setExpanded(this);
    return true;
}