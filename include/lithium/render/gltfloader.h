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

        bool loadJson(const std::filesystem::path& filePath)
        {
            std::ifstream ifs{filePath};
            if(!ifs)
            {
                std::cerr << "Failed to load: " << filePath << std::endl;
                return false;
            }
            ifs >> _json;
            return true;
        }

        lithium::ImageTexture* loadTexture(const std::filesystem::path& filePath)
        {
            lithium::ImageTexture* imageTexture{nullptr};
            for(auto& image : _json["images"])
            {
                const std::string uri = image["uri"].get<std::string>();
                imageTexture = lithium::ImageTexture::load(
                    filePath.parent_path() / uri,
                    GL_SRGB,
                    GL_RGB,
                    GL_LINEAR,
                    GL_CLAMP_TO_EDGE,
                    GL_TEXTURE0,
                    4,
                    false); // flip = false
                break;
            }
            return imageTexture;
        }



        bool loadNodeProperties(const std::filesystem::path& filePath, std::vector<lithium::Node*>& nodeProperties)
        {
            if(!loadJson(filePath))
            {
                return false;
            }

            //loadDataAccessors(filePath);

            loadNodes();

            for(auto entry : _nodeMap)
            {
                lithium::Node* node = entry.second;
                if(node->hasProperties())
                {
                    nodeProperties.push_back(node);
                }
            }

            return true;
        }

        void loadDataAccessors(const std::filesystem::path& filePath)
        {
            auto& buffer = _json["buffers"][0];
            const int bufByteLength = buffer["byteLength"].get<int>();
            const std::string bufUri = buffer["uri"].get<std::string>();
            const std::filesystem::path binPath = filePath.parent_path() / bufUri;

            std::ifstream bufIfs{binPath, std::ios::binary};
            std::vector<uint8_t> bufData{std::istreambuf_iterator<char>(bufIfs), {}};

            size_t numAccessors = static_cast<size_t>(_json["accessors"].size());
            _accessors.resize(numAccessors);

            for(auto& accessor : _json["accessors"])
            {
                const int acsId = accessor["bufferView"].get<int>();
                auto& bufferView = _json["bufferViews"][acsId];
                const int byteLength = bufferView["byteLength"].get<int>();
                const int byteOffset = bufferView["byteOffset"].get<int>();
                int byteEnd = byteOffset + byteLength;
                _accessors[acsId].componentType = accessor["componentType"].get<int>();
                _accessors[acsId].count = accessor["count"].get<int>();
                _accessors[acsId].type = accessor["type"].get<std::string>();
                if(accessor.contains("max"))
                {
                    int j{0};
                    for(auto& value : accessor["max"])
                    {
                        _accessors[acsId].max[j++] = value.get<int>();
                    }
                }

                if(accessor.contains("min"))
                {
                    int j{0};
                    for(auto& value : accessor["min"])
                    {
                        _accessors[acsId].min[j++] = value.get<int>();
                    }
                }

                if(_accessors[acsId].componentType == GL_FLOAT)
                {
                    float* buf = (float*)(bufData.data() + byteOffset);
                    std::copy(&buf[0], &buf[byteLength / 4], std::back_inserter(_accessors[acsId].fData));
                }
                else if(_accessors[acsId].componentType == GL_UNSIGNED_SHORT)
                {
                    uint16_t* buf = (uint16_t*)(bufData.data() + byteOffset);
                    std::copy(&buf[0], &buf[byteLength / 2], std::back_inserter(_accessors[acsId].usData));
                }
                else if(_accessors[acsId].componentType == GL_UNSIGNED_BYTE)
                {
                    std::vector<uint8_t>::const_iterator first = bufData.begin() + byteOffset;
                    std::vector<uint8_t>::const_iterator last = bufData.begin() + byteEnd;
                    std::vector<GLubyte> ubData{first, last};
                    _accessors[acsId].ubData = ubData;
                }
                else
                {
                    GLuint* buf = (GLuint*)(bufData.data() + byteOffset);
                    std::copy(&buf[0], &buf[byteLength / 4], std::back_inserter(_accessors[acsId].usData));
                }
            }
        }

        void loadNodes()
        {
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
                    actualNode->setMeshId(node["mesh"].get<int>());
                }

                if(node.contains("extras"))
                {
                    for(auto& extra : node["extras"].items())
                    {
                        std::cout << extra.key() << "=" << extra.value() << std::endl;
                        if(extra.value().is_array())
                        {
                            actualNode->addPropertyArrayF(extra.key(), extra.value());
                        }
                        else
                        {
                            actualNode->addPropertyF(extra.key(), extra.value());
                        }
                    }
                }

                _nodeMap[i] = actualNode;
            }

            for(auto entry : _nodeMap)
            {
                auto& node = jsonNodes[entry.first];
                if(node.contains("children"))
                {
                    for(auto childId : node["children"])
                    {
                        _nodeMap[childId]->setParent(entry.second);
                    }
                }
            }
        }
        
        lithium::SkinnedObject* loadSkinnedObject(const std::filesystem::path& filePath, bool loadTexture=true)
        {
            if(!loadJson(filePath))
            {
                return nullptr;
            }

            lithium::ImageTexture* imageTexture{nullptr};
            if(loadTexture)
            {
                imageTexture = this->loadTexture(filePath);
            }

            loadDataAccessors(filePath);

            lithium::SkinnedObject* skinnedObj{nullptr};
            lithium::Mesh* skinnedMesh{nullptr};

            for(auto& mesh : _json["meshes"])
            {
                const std::string name{mesh["name"]};
                skinnedMesh = new lithium::Mesh(lithium::VertexArray::DrawFunction::ELEMENTS16);
                skinnedObj = new lithium::SkinnedObject(skinnedMesh, imageTexture);
                for(auto& primitive : mesh["primitives"])
                {
                    skinnedMesh->bind();
                    for(const std::string attr : {"POSITION", "NORMAL", "TEXCOORD_0", "JOINTS_0", "WEIGHTS_0"})
                    {
                        int acsId = primitive["attributes"][attr].get<int>();
                        auto& accessor = _accessors.at(acsId);
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
                    skinnedMesh->createElementArrayBuffer(_accessors.at(acsId).usData);
                }
            }

            loadNodes();

            for(auto entry : _nodeMap)
            {
                skinnedObj->addNode(entry.second);
                if(entry.second->meshId() != -1)
                {
                    skinnedObj->setOwn(entry.second); // Actual mesh.
                }
            }

            int rootNodeId = _json["scenes"][0]["nodes"][0].get<int>();
            lithium::Node* rootNode = _nodeMap.find(rootNodeId)->second;
            //printTree(rootNode, "");

            skinnedObj->setRoot(rootNode);

            auto& skin = _json["skins"][0];
            int inverseBindAcs = skin["inverseBindMatrices"].get<int>();
            std::vector<lithium::Node*> joints;
            for(auto& joint : skin["joints"])
            {
                int j = joint.get<int>();
                joints.push_back(_nodeMap.find(j)->second);
            }
            skinnedObj->skinData(joints, _accessors[inverseBindAcs].fData);

            for(auto& animation: _json["animations"])
            {
                std::vector<float>* input;
                std::vector<float>* output;
                float minTime, maxTime, sPerFrame, fps;
                int frames;
                std::vector<Sampler> samplers;
                for(auto& sampler : animation["samplers"])
                {
                    Accessor& inputAcs = _accessors[sampler["input"].get<int>()];
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
                    Accessor& inputAcs = _accessors[sampler["input"].get<int>()];
                    Accessor& outputAcs = _accessors[sampler["output"].get<int>()];
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
                            frameRotations[i] = glm::quat{sampler.output->at(I + 3),
                            sampler.output->at(I + 0),
                            sampler.output->at(I + 1),
                            sampler.output->at(I + 2)};
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
        std::vector<Accessor> _accessors;
        std::map<int,lithium::Node*> _nodeMap;
    };
}