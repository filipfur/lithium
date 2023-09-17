#pragma once

#include <fstream>
#include <map>
#include <filesystem>

//#include <glm/gtx/quaternion.hpp>
#include "gljson.h"

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
            this->_json.clear();
            this->_accessors.clear();
            this->_nodes.clear();
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
            if(!_json.contains("images"))
            {
                return imageTexture;
            }
            for(auto& image : _json["images"])
            {
                const std::string uri = image["uri"].get<std::string>();
                imageTexture = lithium::ImageTexture::load(
                    filePath.parent_path() / uri,
                    GL_SRGB,
                    GL_RGB,
                    4,
                    false); // flip = false
                break;
            }
            return imageTexture;
        }



        bool loadNodesWithProperties(const std::filesystem::path& filePath, std::vector<lithium::Node*>& nodes)
        {
            if(!loadJson(filePath))
            {
                return false;
            }

            loadNodes();

            for(lithium::Node* node : _nodes)
            {
                if(node->hasProperties())
                {
                    nodes.push_back(node);
                }
            }

            return true;
        }

        bool loadNodes(const std::filesystem::path& filePath, std::vector<lithium::Node*>& nodes)
        {
            if(!loadJson(filePath))
            {
                return false;
            }

            //loadDataAccessors(filePath);

            loadNodes();

            for(lithium::Node* node : _nodes)
            {
                nodes.push_back(node);
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
                    for(auto& extra : node["extras"].children())
                    {
                        std::cout << extra.first << "=" << extra.second.value() << std::endl;
                        if(extra.second.isArray())
                        {
                            std::vector<GLfloat> values;
                            for(auto& value : extra.second)
                            {
                                values.push_back(value.get<float>());
                            }
                            actualNode->addPropertyArrayF(extra.first, values);
                        }
                        else
                        {
                            actualNode->addPropertyF(extra.first, extra.second.get<float>());
                        }
                    }
                }
                _nodes.push_back(actualNode);
                assert(_nodes.size() == i + 1);
            }

            for(int n{0}; n < _nodes.size(); ++n)
            {
                auto& node = jsonNodes[n];
                if(node.contains("children"))
                {
                    for(auto childId : node["children"])
                    {
                        _nodes.at(childId.get<int>())->setParent(_nodes[n]);
                    }
                }
            }
        }

        void loadMaterials(std::vector<std::shared_ptr<lithium::Material>>& materials,
            const std::filesystem::path& filePath)
        {
            std::vector<std::shared_ptr<lithium::ImageTexture>> images;
            if(_json.contains("images"))
            {
                for(auto& image : _json["images"])
                {
                    const std::string uri = image["uri"].get<std::string>();
                    images.push_back(std::shared_ptr<lithium::ImageTexture>(lithium::ImageTexture::load(
                        filePath.parent_path() / uri,
                        GL_SRGB,
                        GL_RGB,
                        4,
                        false))); // flip = false
                }
            }

            std::vector<int> textures;
            if(_json.contains("textures"))
            {
                for(auto& texture : _json["textures"])
                {
                    textures.push_back(texture["source"].get<int>());
                }
            }

            auto lookupTexture = [&](lithium::json::Json& json) {
                int sourceIndex = textures.at(json["index"].get<int>());
                return images.at(sourceIndex);
            };

            if(_json.contains("materials"))
            {
                for(auto& material : _json["materials"])
                {
                    std::shared_ptr<lithium::Material> mat = std::make_shared<lithium::Material>(glm::vec4{1.0f});
                    if(material.contains("normalTexture"))
                    {
                        mat->setNormalMap(lookupTexture(material["normalTexture"]));
                    }
                    if(material.contains("pbrMetallicRoughness"))
                    {
                        auto& pbr = material["pbrMetallicRoughness"];
                        if(pbr.contains("baseColorFactor"))
                        {
                            auto& baseColorFactor = pbr["baseColorFactor"];
                            mat->setBaseColor(glm::vec4{baseColorFactor[0].get<float>(),
                                baseColorFactor[1].get<float>(),
                                baseColorFactor[2].get<float>(),
                                baseColorFactor[3].get<float>()});
                        }
                        if(pbr.contains("baseColorTexture"))
                        {
                            mat->setDiffuseMap(lookupTexture(pbr["baseColorTexture"]));
                        }
                        if(pbr.contains("metallicFactor"))
                        {
                            mat->setMetallic(pbr["metallicFactor"].get<float>());
                        }
                        if(pbr.contains("roughnessFactor"))
                        {
                            mat->setRoughness(pbr["roughnessFactor"].get<float>());
                        }
                        if(pbr.contains("metallicRoughnessTexture"))
                        {
                            mat->setArmMap(lookupTexture(pbr["metallicRoughnessTexture"]));
                        }
                    }
                    materials.push_back(mat);
                }
            }
        }

        void loadObjects(std::vector<std::shared_ptr<lithium::Object>>& objects,
            const std::filesystem::path& filePath)
        {
            if(!loadJson(filePath))
            {
                return;
            }

            std::vector<std::shared_ptr<lithium::Material>> materials;
            std::vector<std::shared_ptr<lithium::Mesh>> meshes;

            loadMaterials(materials, filePath);

            loadDataAccessors(filePath);

            for(auto& mesh : _json["meshes"])
            {
                const std::string name{mesh["name"]};
                auto retMesh = std::make_shared<lithium::Mesh>(lithium::VertexArray::DrawFunction::ELEMENTS16);
                for(auto& primitive : mesh["primitives"])
                {
                    retMesh->bind();
                    for(const std::string attr : {"POSITION", "NORMAL", "TEXCOORD_0"})
                    {
                        int acsId = primitive["attributes"][attr].get<int>();
                        auto& accessor = _accessors.at(acsId);
                        if(accessor.componentType == GL_UNSIGNED_BYTE)
                        {
                            retMesh->createArrayBuffer({toAttributeType(accessor.type)}, accessor.ubData, accessor.componentType);
                        }
                        else if(accessor.componentType == GL_UNSIGNED_INT)
                        {
                            retMesh->createArrayBuffer({toAttributeType(accessor.type)}, accessor.uiData, accessor.componentType);
                        }
                        else if(accessor.componentType == GL_UNSIGNED_SHORT)
                        {
                            retMesh->createArrayBuffer({toAttributeType(accessor.type)}, accessor.usData, accessor.componentType);
                        }
                        else
                        {
                            retMesh->createArrayBuffer({toAttributeType(accessor.type)}, accessor.fData, accessor.componentType);
                        }
                    }
                    int acsId = primitive["indices"].get<int>();
                    retMesh->createElementArrayBuffer(_accessors.at(acsId).usData);
                    if(primitive.contains("material"))
                    {
                        retMesh->setMaterial(materials.at(primitive["material"].get<int>()));
                    }
                }
                meshes.push_back(retMesh);
            }

            loadNodes();

            static std::shared_ptr<lithium::Texture<unsigned char>> defaultTexture = lithium::Texture<unsigned char>::Basic();

            for(lithium::Node* node : _nodes)
            {
                auto mesh = meshes.at(node->meshId());
                std::vector<lithium::Object::TexturePointer> textures;
                if(mesh->material()->diffuseMap())
                {
                    textures.push_back(mesh->material()->diffuseMap());
                }
                if(mesh->material()->normalMap())
                {
                    textures.push_back(mesh->material()->normalMap());
                }
                if(mesh->material()->armMap())
                {
                    textures.push_back(mesh->material()->armMap());
                }
                auto retObj = std::make_shared<lithium::Object>(lithium::Object(mesh, textures));
                retObj->setObjectName(node->name());
                retObj->setPosition(node->position());
                retObj->setQuaternion(node->rotation());
                retObj->setScale(node->scale());
                objects.push_back(retObj);
            }
        }
        
        std::shared_ptr<lithium::SkinnedObject> loadSkinnedObject(const std::filesystem::path& filePath, bool loadTexture=true)
        {
            if(!loadJson(filePath))
            {
                return nullptr;
            }

            std::shared_ptr<lithium::ImageTexture> imageTexture{nullptr};
            if(loadTexture)
            {
                imageTexture.reset(this->loadTexture(filePath));
            }

            loadDataAccessors(filePath);

            std::shared_ptr<lithium::SkinnedObject> skinnedObj{nullptr};
            std::shared_ptr<lithium::Mesh> skinnedMesh{nullptr};

            for(auto& mesh : _json["meshes"])
            {
                const std::string name{mesh["name"]};
                skinnedMesh = std::make_shared<lithium::Mesh>(lithium::VertexArray::DrawFunction::ELEMENTS16);
                skinnedObj.reset(new lithium::SkinnedObject(skinnedMesh, {imageTexture}));
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

            for(lithium::Node* node : _nodes)
            {
                skinnedObj->addNode(node);
                if(node->meshId() != -1)
                {
                    skinnedObj->setOwn(node); // Actual mesh.
                }
            }

            int rootNodeId = _json["scenes"][0]["nodes"][0].get<int>();
            lithium::Node* rootNode = _nodes.at(rootNodeId);
            //printTree(rootNode, "");

            skinnedObj->setRoot(rootNode);

            auto& skin = _json["skins"][0];
            int inverseBindAcs = skin["inverseBindMatrices"].get<int>();
            std::vector<lithium::Node*> joints;
            for(auto& joint : skin["joints"])
            {
                int j = joint.get<int>();
                joints.push_back(_nodes.at(j));
            }
            skinnedObj->skinData(joints, _accessors[inverseBindAcs].fData);

            for(auto& animation: _json["animations"])
            {
                std::vector<float>* input;
                std::vector<float>* output;
                const std::string animName = animation["name"].get<std::string>();
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
                lithium::SkinAnimation* skinAnimation = new lithium::SkinAnimation(animName,
                    frames, translationsMap, rotationsMap);
                std::cout << "animName=" << animName << ", frames=" << frames << std::endl;
                
                skinAnimation->setInterval(sPerFrame);
                skinnedObj->addSkinAnimation(skinAnimation);
            }

            return skinnedObj;
        }

    private:
        lithium::json::Json _json;
        std::vector<Accessor> _accessors;
        std::vector<lithium::Node*> _nodes;
    };
}