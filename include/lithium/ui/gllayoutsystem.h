#pragma once

#include <functional>
#include "glframelayout.h"
#include "gljson.h"

namespace lithium
{
    class LayoutSystem
    {
    public:
        LayoutSystem();
        
        LayoutSystem(const lithium::json::Json& json);

        void load(const lithium::json::Json& json);

        FrameLayout* root()
        {
            return &_root;
        }

        void forEachFrameLayout(std::function<void(FrameLayout&)> callback)
        {
            callback(_root);
            forEachChild(_root, callback);
        }

        void updateLayouts();

    private:
        void parse(const lithium::json::Json& json, FrameLayout& frameLayout);

        void forEachChild(FrameLayout& frameLayout, std::function<void(FrameLayout&)> callback)
        {
            for(auto& child : frameLayout._children)
            {
                callback(child);
                forEachChild(child, callback);
            }
        }
        
        FrameLayout _root;
    };
}