#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include "gljson.h"

const char* testJson = R"({
    "name": "John Doe",
    "age": 43,
    "address": {
        "street": "123 Main Street",
        "city": "Anytown",
        "state": "NY",
        "postal": 12345
    },
    "phone": [
        {
            "type": "home",
            "number": "555-555-5555"
        },
        {
            "type": "fax",
            "number": "555-555-5556"
        }
    ],
    "arr": [
        0.12562,
        0.774,
        1.242
    ],
    "inters": [1,   2, ,4,8]
}
)";

int main(int argc, const char* argv[])
{
    /*std::ifstream ifs{"test.json"};

    if(!ifs) {
        std::cerr << "failed to open stooped file.";
        return 1;
    }

    lithium::json::Json obj;

    ifs >> obj;

    ifs.close();*/


    lithium::json::Json obj;

    std::stringstream ss{testJson};

    ss >> obj;

    //std::cout << obj << std::endl;

    std::stringstream ss2;

    ss2 << obj;

    //std::cout << ss2.str() << std::endl;

    lithium::json::Json obj2;

    ss2 >> obj2;

    std::cout << obj2 << std::endl;

    std::cout << "All tests passed!" << std::endl;

    assert(obj2["name"].value() == "John Doe");
    assert(obj2["age"].get<int>() == 43);

    assert(obj2["address"]["street"].get<std::string>() == "123 Main Street");
    assert(obj2["address"]["city"].get<std::string>() == "Anytown");
    assert(obj2["address"]["state"].get<std::string>() == "NY");
    assert(obj2["address"]["postal"].get<int>() == 12345);

    assert(obj2["phone"][0]["type"].get<std::string>() == "home");
    assert(obj2["phone"][0]["number"].get<std::string>() == "555-555-5555");
    assert(obj2["phone"][1]["type"].get<std::string>() == "fax");
    assert(obj2["phone"][1]["number"].get<std::string>() == "555-555-5556");

    assert(obj2["arr"][0].get<double>() == 0.12562);
    assert(obj2["arr"][1].get<float>() == 0.774f);
    assert(obj2["arr"][2].get<float>() == 1.242f);

    for(auto& o : obj2["inters"])
    {
        std::cout << static_cast<int>(o) << std::endl;
    }

    return 0;
}