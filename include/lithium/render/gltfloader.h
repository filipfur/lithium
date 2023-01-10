#include "nlohmann/json.hpp"

namespace gltf
{
    class Loader
    {
    public:
        Loader()
        {

        }

    private:
        nlohmann::json _json;

    };
}