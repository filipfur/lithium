#include <fstream>
#include <map>
#include <filesystem>

#include "nlohmann/json.hpp"

#include "glnode.h"
#include "glskinnedobject.h"

namespace gltf
{
    class Loader
    {
        struct Accessor
        {
            uint8_t* start;
            int length;
            int componentType;
            int count;
            std::string type;
        };


    public:
        Loader()
        {

        }

        virtual ~Loader() noexcept
        {

        }

        void printTree(lithium::Node* node, const std::string indent)
        {
            node->forAllChildren([&](lithium::Node* child) {
                std::cout << indent << child->name() << std::endl;
                printTree(child, indent + "  ");
            });
        }

        void createBuffer(nlohmann::json& json)
        {

        }
        
        lithium::SkinnedObject* load(const std::filesystem::path& filePath)
        {
            std::ifstream ifs{filePath};
            ifs >> _json;
            std::cout << "scenes: " << _json["scenes"] << std::endl;

            std::map<int,lithium::Node*> nodeMap;

            auto& buffer = _json["buffers"][0];

            std::cout << buffer << std::endl;
            const int bufByteLength = buffer["byteLength"].get<int>();
            const std::string bufUri = buffer["uri"].get<std::string>();
            const std::filesystem::path binPath = filePath.parent_path() / bufUri;

            std::vector<uint8_t> bufData;
            bufData.reserve(bufByteLength);
            std::ifstream bufIfs{binPath, std::ios::binary};
            std::copy(std::istream_iterator<uint8_t>(bufIfs), std::istream_iterator<uint8_t>(), std::back_inserter(bufData));

            /*for(uint8_t val : bufData)
            {
                std::cout << val << ' ';
            }*/

            const int numAccessors = _json["accessors"].size();
            Accessor accessors[numAccessors];

            for(auto& accessor : _json["accessors"])
            {
                std::cout << accessor << std::endl;
                const int acsId = accessor["bufferView"].get<int>();
                auto& bufferView = _json["bufferViews"][acsId];
                const int byteLength = bufferView["byteLength"].get<int>();
                const int byteOffset = bufferView["byteOffset"].get<int>();
                accessors[acsId].length = byteLength;
                accessors[acsId].start = bufData.data() + byteOffset;
                accessors[acsId].componentType = accessor["componentType"].get<int>();
                accessors[acsId].count = accessor["count"].get<int>();
                accessors[acsId].type = accessor["type"].get<std::string>();
            }

            lithium::SkinnedObject* skinnedObj{nullptr};

            for(auto& mesh : _json["meshes"])
            {
                const std::string name{mesh["name"]};
                skinnedObj = new lithium::SkinnedObject();
                for(auto& primitive : mesh["primitives"])
                {

                }
            }

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
                auto& node = jsonNodes[entry.first];
                if(node.contains("children"))
                {
                    for(auto childId : node["children"])
                    {
                        nodeMap[childId]->setParent(entry.second);
                    }
                }
            }

            int rootNodeId = _json["scenes"][0]["nodes"][0].get<int>();
            lithium::Node* rootNode = nodeMap.find(rootNodeId)->second;
            std::cout << rootNode->name() << std::endl;
            printTree(rootNode, "");

            return nullptr;
        }

    private:
        nlohmann::json _json;

    };
}