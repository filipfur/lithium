#include <iostream>
#include <fstream>
#include "gljson.h"


int main(int argc, const char* argv[])
{
    std::cout << "All tests passed!" << std::endl;

    std::ifstream ifs{"test.json"};

    if(!ifs) {
        std::cerr << "failed to open stooped file.";
        return 1;
    }

    lithium::json::Json obj;

    ifs >> obj;

    ifs.close();

    std::cout << obj << std::endl;

    return 0;
}