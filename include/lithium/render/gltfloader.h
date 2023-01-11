#include <fstream>

#include "nlohmann/json.hpp"

#include "glskinnedobject.h"

namespace gltf
{
    class Loader
    {
    public:
        Loader()
        {

        }

        virtual ~Loader() noexcept
        {

        }
        
        lithium::SkinnedObject* load(const std::string& filePath)
        {
            std::ifstream ifs{filePath};
            ifs >> _json;
            std::cout << "scenes: " << _json["scenes"] << std::endl;

            for(node : _json["nodes"].get())
            {

            }

            return nullptr;
        }

    private:
        nlohmann::json _json;

    };
}