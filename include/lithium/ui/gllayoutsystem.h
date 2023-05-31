#pragma once

#include "glframelayout.h"
#include "gljson.h"

namespace lithium
{
    class LayoutSystem
    {
    public:
        LayoutSystem(const lithium::json::Json& json);

    private:
        void parse(const lithium::json::Json& json, FrameLayout& frameLayout);

        FrameLayout _root;
    };
}