#pragma once

namespace lithium
{
    class Application;
    class Input;
    struct UserPointer
    {
        Application* application{nullptr};
        Input* input{nullptr};
    };
}
