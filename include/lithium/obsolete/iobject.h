#pragma once

namespace lithium
{
    class Object;

    class IObject
    {
    public:
        virtual void onDoneFlickering(lithium::Object* animatedObject) = 0;
    };
}