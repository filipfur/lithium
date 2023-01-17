#include <fstream>
#include <map>
#include <filesystem>

//#include <glm/gtx/quaternion.hpp>
#include "nlohmann/json.hpp"

#include "glnode.h"
#include "glskinnedobject.h"

namespace gltf
{
    class Loader
    {
        struct Accessor
        {
            std::vector<GLfloat> fData;
            std::vector<GLushort> usData;
            std::vector<GLuint> uiData;
            std::vector<GLubyte> ubData;
            glm::vec3 min{0.0f};
            glm::vec3 max{0.0f};
            int componentType;
            int count;
            std::string type;
        };

        struct Sampler
        {
            std::vector<GLfloat>* input{nullptr};
            std::vector<GLfloat>* output{nullptr};
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

        lithium::VertexArrayBuffer::AttributeType toAttributeType(const std::string& type)
        {
            if(type == "MAT4")
            {
                return lithium::VertexArrayBuffer::AttributeType::MAT4;
            }
            else if(type == "MAT3")
            {
                return lithium::VertexArrayBuffer::AttributeType::MAT3;
            }
            else if(type == "VEC4")
            {
                return lithium::VertexArrayBuffer::AttributeType::VEC4;
            }
            else if(type == "VEC3")
            {
                return lithium::VertexArrayBuffer::AttributeType::VEC3;
            }
            else if(type == "VEC2")
            {
                return lithium::VertexArrayBuffer::AttributeType::VEC2;
            }
            else if(type == "SCALAR")
            {
                return lithium::VertexArrayBuffer::AttributeType::FLOAT;
            }
            return lithium::VertexArrayBuffer::AttributeType::FLOAT;
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

            //bufData.reserve(bufByteLength);
            std::ifstream bufIfs{binPath, std::ios::binary};
            std::vector<uint8_t> bufData{std::istreambuf_iterator<char>(bufIfs), {}};
            //std::copy(std::istream_iterator<uint8_t>(bufIfs), std::istream_iterator<uint8_t>(), std::back_inserter(bufData));

            /*for(uint8_t val : bufData)
            {
                std::cout << val << ' ';
            }*/

            size_t numAccessors = static_cast<size_t>(_json["accessors"].size());
            std::vector<Accessor> accessors{numAccessors};

            for(auto& accessor : _json["accessors"])
            {
                const int acsId = accessor["bufferView"].get<int>();
                auto& bufferView = _json["bufferViews"][acsId];
                const int byteLength = bufferView["byteLength"].get<int>();
                const int byteOffset = bufferView["byteOffset"].get<int>();
                int byteEnd = byteOffset + byteLength;
                accessors[acsId].componentType = accessor["componentType"].get<int>();
                accessors[acsId].count = accessor["count"].get<int>();
                accessors[acsId].type = accessor["type"].get<std::string>();
                if(accessor.contains("max"))
                {
                    int j{0};
                    for(auto& value : accessor["max"])
                    {
                        accessors[acsId].max[j++] = value.get<int>();
                    }
                }

                if(accessor.contains("min"))
                {
                    int j{0};
                    for(auto& value : accessor["min"])
                    {
                        accessors[acsId].min[j++] = value.get<int>();
                    }
                }


                std::cout << acsId << std::endl;
                if(accessors[acsId].componentType == GL_FLOAT)
                {
                    //std::vector<uint8_t>::const_iterator first = bufData.begin() + byteOffset;
                    //std::vector<uint8_t>::const_iterator last = bufData.begin() + byteEnd;
                    float* buf = (float*)(bufData.data() + byteOffset);
                    std::copy(&buf[0], &buf[byteLength / 4], std::back_inserter(accessors[acsId].fData));

                    //accessors[acsId].data.reserve(byteLength);
                    //memcpy(accessors[acsId].data.data(), bufData.data() + byteOffset, byteLength);
                }
                else if(accessors[acsId].componentType == GL_UNSIGNED_SHORT)
                {
                    uint16_t* buf = (uint16_t*)(bufData.data() + byteOffset);
                    std::copy(&buf[0], &buf[byteLength / 2], std::back_inserter(accessors[acsId].usData));
                }
                else if(accessors[acsId].componentType == GL_UNSIGNED_BYTE)
                {
                    std::vector<uint8_t>::const_iterator first = bufData.begin() + byteOffset;
                    std::vector<uint8_t>::const_iterator last = bufData.begin() + byteEnd;
                    std::vector<GLubyte> ubData{first, last};
                    accessors[acsId].ubData = ubData;
                }
                else
                {
                    GLuint* buf = (GLuint*)(bufData.data() + byteOffset);
                    std::copy(&buf[0], &buf[byteLength / 4], std::back_inserter(accessors[acsId].usData));

                    //accessors[acsId].iData.reserve(byteLength);
                    //memcpy(accessors[acsId].iData.data(), bufData.data() + byteOffset, byteLength);
                }
            }

            lithium::SkinnedObject* skinnedObj{nullptr};
            lithium::Mesh* skinnedMesh{nullptr};

            for(auto& mesh : _json["meshes"])
            {
                const std::string name{mesh["name"]};
                skinnedMesh = new lithium::Mesh(lithium::VertexArray::DrawFunction::ELEMENTS16);
                skinnedObj = new lithium::SkinnedObject(skinnedMesh, nullptr);
                for(auto& primitive : mesh["primitives"])
                {
                    skinnedMesh->bind();
                    for(const std::string attr : {"POSITION", "NORMAL", "TEXCOORD_0", "JOINTS_0", "WEIGHTS_0"})
                    {
                        int acsId = primitive["attributes"][attr].get<int>();
                        auto& accessor = accessors.at(acsId);
                        if(accessor.componentType == GL_UNSIGNED_BYTE)
                        {
                            skinnedMesh->createArrayBuffer({toAttributeType(accessor.type)}, accessor.ubData, accessor.componentType);
                        }
                        else if(accessor.componentType == GL_UNSIGNED_INT)
                        {
                            skinnedMesh->createArrayBuffer({toAttributeType(accessor.type)}, accessor.uiData, accessor.componentType);
                        }
                        else if(accessor.componentType == GL_UNSIGNED_SHORT)
                        {
                            skinnedMesh->createArrayBuffer({toAttributeType(accessor.type)}, accessor.usData, accessor.componentType);
                        }
                        else
                        {
                            skinnedMesh->createArrayBuffer({toAttributeType(accessor.type)}, accessor.fData, accessor.componentType);
                        }
                    }
                    int acsId = primitive["indices"].get<int>();
                    skinnedMesh->createElementArrayBuffer(accessors.at(acsId).usData);
                }
            }

            auto& jsonNodes = _json["nodes"];
            for(int i{0}; i < jsonNodes.size(); ++i)
            {
                auto& node = jsonNodes[i];
                const std::string nodeName = node["name"].get<std::string>();
                glm::vec3 position{0.0f};
                glm::quat rotation{1.0f, 0.0f, 0.0f, 0.0f};
                glm::vec3 scale{1.0f};

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
                lithium::Node* actualNode = new lithium::Node(i, nodeName, position, rotation, scale);
                if(node.contains("mesh"))
                {
                    skinnedObj->setOwn(actualNode); // Actual mesh.
                }
                nodeMap[i] = actualNode;
            }


            for(auto entry : nodeMap)
            {
                auto& node = jsonNodes[entry.first];
                skinnedObj->addNode(entry.second);
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

            skinnedObj->setRoot(rootNode);

            auto& skin = _json["skins"][0];
            int inverseBindAcs = skin["inverseBindMatrices"].get<int>();
            std::cout << "invBindAcs: " << inverseBindAcs << std::endl;
            std::vector<lithium::Node*> joints;
            for(auto& joint : skin["joints"])
            {
                int j = joint.get<int>();
                std::cout << "joint: " << joint << std::endl;
                joints.push_back(nodeMap.find(j)->second);
            }
            skinnedObj->skinData(joints, accessors[inverseBindAcs].fData);

            for(auto& animation: _json["animations"])
            {
                std::cout << "Animation: " << animation["name"] << std::endl;
                std::vector<float>* input;
                std::vector<float>* output;
                float minTime, maxTime, sPerFrame, fps;
                int frames;
                std::vector<Sampler> samplers;
                for(auto& sampler : animation["samplers"])
                {
                    Accessor& inputAcs = accessors[sampler["input"].get<int>()];
                    input = &inputAcs.fData;
                    minTime = inputAcs.min[0];
                    maxTime = inputAcs.max[0];
                    frames = input->size();
                    sPerFrame = maxTime / frames;
                    fps = 1.0f / sPerFrame;
                    break;
                }
                for(auto& sampler : animation["samplers"])
                {
                    Accessor& inputAcs = accessors[sampler["input"].get<int>()];
                    Accessor& outputAcs = accessors[sampler["output"].get<int>()];
                    input = &inputAcs.fData;     
                    output = &outputAcs.fData;
                    samplers.push_back(Sampler{input, output});
                }

                std::map<int, std::vector<glm::vec3>> translationsMap;
                std::map<int, std::vector<glm::quat>> rotationsMap;

                for(auto& channel : animation["channels"])
                {
                    auto& sampler = samplers[channel["sampler"].get<int>()];
                    const std::string targetPath = channel["target"]["path"].get<std::string>();
                    const int targetNode = channel["target"]["node"].get<int>();
                    if(targetPath == "translation")
                    {
                        std::vector<glm::vec3> frameTranslations{static_cast<size_t>(frames)};
                        for(int i{0}; i < frames; ++i)
                        {
                            int I = i * 3;
                            frameTranslations[i] = glm::vec3{sampler.output->at(I), sampler.output->at(I + 1), sampler.output->at(I + 2)};
                        }
                        translationsMap.emplace(targetNode, frameTranslations);
                    }
                    else if(targetPath == "rotation")
                    {
                        std::vector<glm::quat> frameRotations{static_cast<size_t>(frames)};
                        for(int i{0}; i < frames; ++i)
                        {
                            int I = i * 4;
                            frameRotations[i] = glm::quat{sampler.output->at(I),
                            sampler.output->at(I + 1),
                            sampler.output->at(I + 2),
                            sampler.output->at(I + 3)};
                        }
                        rotationsMap.emplace(targetNode, frameRotations);
                    }
                    else if(targetPath == "scale")
                    {

                    }
                }
                lithium::SkinAnimation* skinAnimation = new lithium::SkinAnimation(animation["name"].get<std::string>(),
                    frames, translationsMap, rotationsMap);
                skinAnimation->setInterval(sPerFrame);
                skinnedObj->addSkinAnimation(skinAnimation);
            }

            return skinnedObj;
        }

    private:
        nlohmann::json _json;

    };
}