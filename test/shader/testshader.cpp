#include <iostream>
#include "glshader.h"

int main(int argc, const char* argv[])
{   
    printf("Testing Shader class...\n");
    const std::filesystem::path path{"lithium/test/shader/test.frag"};
    auto src = lithium::FragmentShader::readFile(path.string());
    auto expanded = lithium::FragmentShader::expandSource(path.parent_path(), src);
    std::cout << expanded << std::endl;
    return 0;
}