#include <iostream>

#include "glfilewatch.h"

class Test
{

};

class A : public Test
{

};

class B : public Test
{

};

void test(A* a)
{
    std::cout << "A" << std::endl;
}

void test(B* b)
{
    std::cout << "B" << std::endl;
}

int main(int argc, const char* argv[])
{
    for(auto& f : fs::directory_iterator("input"))
    {
        lithium::FileWatch::start(f.path(), [](const fs::path& path) {
            std::cout << "File changed: " << path.string() << std::endl;
        });
    }
    std::cin.get();

    return 0;
}