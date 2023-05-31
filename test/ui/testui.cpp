#include <iostream>
#include <cassert>
#include <sstream>
#include "gljson.h"
#include "gllayoutsystem.h"

const char* uiSrc = R"({
    "mode": "fixed",
    "orientation": "horizontal",
    "alignment": "left",
    "margin": [64.0, 32.0],
    "padding": [32.0],
    "dimension": [128.0, 32.0],
    "position": [10.0, 0.0],
    "children": [
        {
            "mode": "fill",
            "orientation": "vertical",
            "alignment": "center",
            "margin": [32.0],
            "padding": [8.0, 16.0, 32.0, 64.0],
            "dimension": [64.0, 64.0]
        },
        {
            "mode": "wRAP",
            "orientation": "HORIZONTAL",
            "alignment": "Bottom",
            "margin": [32.0],
            "padding": [16.0, 16.0, 32.0, 32.0],
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