#include <iostream>
#include <cassert>
#include <sstream>
#include "gljson.h"
#include "gllayoutsystem.h"

const char* uiSrc = R"({
    "mode": "absolute",
    "margin": [0.0],
    "padding": [0.0],
    "dimension": [1024.0, 1024.0],
    "position": [0.0, 0.0],
    "children": [
        {
            "mode": "absolute",
            "margin": [0.0],
            "padding": [8.0],
            "dimension": [128.0, 128.0],
            "position": [256.0, 256.0]
        },
        {
            "mode": "absolute",
            "margin": [0.0],
            "padding": [8.0],
            "dimension": [128.0, 128.0],
            "position": [-256.0, 512.0]
        }
    ]
}
)";

int main(int argc, const char* argv[])
{
    std::cout << "Hello World!" << std::endl;

    std::stringstream ss{uiSrc};
    lithium::json::Json obj;
    // Perhaps create a >> std::fixed style api that can transform keys to lower case. Applicable for istream?
    // Also create a std::fixed << style api that can set indent width.
    ss >> obj;
    std::cout << obj << std::endl;

    lithium::LayoutSystem layoutSystem{obj};

    return 0;
}