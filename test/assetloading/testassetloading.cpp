#include <iostream>

#include "glfilesystem.h"

int main(int argc, const char* argv[])
{
    lithium::fs::forEachInDirectory("lithium/test/assetloading/assets", [](const std::filesystem::path& path)
    {
        std::cout << path.filename().string() << std::endl;
    });

    return 0;
}