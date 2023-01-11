#include <fstream>
#include <map>

#include "nlohmann/json.hpp"

#include "glnode.h"
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

            std::map<int,lithium::Node*> nodeMap;

            lithium::Node* rootNode = new lithium::Node("", glm::vec3{0.0f}, glm::quat{1.0f, 0.0f, 0.0f, 0.0f}, glm::vec3{0.0f});

            nodeMap[-1] = rootNode;

            auto& jsonNodes = _json["nodes"];

            for(int i{0}; i < jsonNodes.size(); ++i)
            {
                auto& node = jsonNodes[i];
                const std::string nodeName = node["name"].get<std::string>();
                glm::vec3 position{0.0f};
                glm::quat rotation{1.0f, 0.0f, 0.0f, 0.0f};
                glm::vec3 scale{0.0f};

                if(node.contains("translation"))
                {
                    position.x = node["translation"][0].get<float>();
                    position.y = node["translation"][1].get<float>();
                    position.z = node["translation"][2].get<float>();
                }
                if(node.contains("rotation"))
                {
                    rotation.x = node["rotation"][0].get<float>();
                    rotation.y = node["rotation"][1].get<float>();
                    rotation.z = node["rotation"][2].get<float>();
                    rotation.w = node["rotation"][3].get<float>();
                }
                if(node.contains("scale"))
                {
                    scale.x = node["scale"][0].get<float>();
                    scale.y = node["scale"][1].get<float>();
                    scale.z = node["scale"][2].get<float>();
                }
                lithium::Node* actualNode = new lithium::Node(nodeName, position, rotation, scale);
                nodeMap[i] = actualNode;
            }

            for(auto entry : nodeMap)
            {
                
            }

            return nullptr;
        }

    private:
        nlohmann::json _json;

    };
}