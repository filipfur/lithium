#pragma once

namespace mygl
{
    class Object;

    class IObject
    {
    public:
        virtual void onDoneFlickering(mygl::Object* animatedObject) = 0;
    };
}