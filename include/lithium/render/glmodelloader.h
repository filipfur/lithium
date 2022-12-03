
#pragma once

#include <filesystem>
#include "glmodel.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "gltimemeasure.h"


#define MAX_BONE_INFLUENCE 4
#define __dirname(path) path.substr(0, path.find_last_of("/"))
#define __filename(path) path.substr(path.find_last_of('/') + 1, std::string::npos)
#define __noextension(path) path.substr(0, path.find_first_of('.'))

//#define MODELLOADER_VERTEX_LOG

namespace lithium
{
    class ModelLoader
    {
    public:
        ModelLoader()
        {

        }

        lithium::Model* load(std::string const &path, lithium::Mesh::State state=lithium::Mesh::State::POS_NORMAL_UV_TANGENTS_BONE_WEIGHT)
        {
#ifdef MODELLOADER_VERTEX_LOG
            _vertexLog.open(__dirname(path) + "/vertexlog.txt");
#endif
            lithium::TimeMeasure::Handle handle{lithium::TimeMeasure::start("loading model " + std::string(path), true)};
            const std::filesystem::path filePath{path};
            _directory = filePath.parent_path();
            const std::string name = _directory.filename().u8string();
            lithium::Model* model = new lithium::Model(name);
            std::filesystem::path animDir = _directory / "animations";

            loadModel(model, path, state);
            if(std::filesystem::exists(animDir))
            {
                for(const auto& entry : std::filesystem::directory_iterator(animDir))
                {
                    const std::string ext = entry.path().extension().string();
                    if(ext == ".dae")
                    {
                        loadAnimation(model, entry.path(), 0);
                    }
                }
            }
            model->playAnimation(0);
            _models.emplace(name, model);
            return model;
        }

        void unload(const std::string& name)
        {
            auto it = _models.find(name);
            delete it->second;
            _models.erase(name);
        }

        lithium::Model* clone(const std::string& name)
        {
            auto it = _models.find(name);
            lithium::Model* model{nullptr};
            if(it != _models.end())
            {
                model = it->second->clone();
            }
            return model;
        }

        lithium::Model* getModel(const std::string& modelName)
        {
            lithium::Model* model{nullptr};
            auto it = _models.find(modelName);
            if(it != _models.end())
            {
                model = it->second;
            }
            return model;
        }
        
        virtual ~ModelLoader() noexcept
        {

        }

        std::string loadAnimation(lithium::Model* model, const std::filesystem::path path, size_t index)
        {
            std::string animName =  path.stem().u8string();
            auto animation = new lithium::Animation(path.u8string(), model->m_BoneInfoMap, index);
            auto it = model->_animations.find(animName);
            if(it != model->_animations.end())
            {
                delete it->second;
                model->_animations.erase(animName);
            }
            model->_animations.emplace(animName, animation);
            return animName;
        }

    private:
        // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
        void loadModel(lithium::Model* model, std::string const &path, lithium::Mesh::State state)
        {
            // read file via ASSIMP
            Assimp::Importer importer;
            const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate); //| aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);
            // check for errors
            if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
            {
                std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
                return;
            }

            // process ASSIMP's root node recursively
            processNode(model, scene->mRootNode, scene, state);
        }

        // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
        void processNode(lithium::Model* model, aiNode *node, const aiScene *scene, lithium::Mesh::State state)
        {
            // process each mesh located at the current node
            for(unsigned int i = 0; i < node->mNumMeshes; i++)
            {
                // the node object only contains indices to index the actual objects in the scene. 
                // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
                aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
                if(mesh->mNumVertices < 5)
                {
                    std::cerr << "Discarding very little mesh: " << mesh->mName.C_Str() << std::endl;
                }
                else
                {
                    model->_objects.push_back(processMesh(model, mesh, scene, state));
                }
            }
            // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
            for(unsigned int i = 0; i < node->mNumChildren; i++)
            {
                processNode(model, node->mChildren[i], scene, state);
            }

        }

        struct MeshVertex
        {
            glm::vec3 pos;
            glm::vec3 normal;
            glm::vec2 tex;
            glm::vec3 tangent;
            glm::vec3 bitangent;
            float boneIds[MAX_BONE_INFLUENCE];
            float boneWeights[MAX_BONE_INFLUENCE];
        };


        void compute_tangents_lengyel(MeshVertex* pVertices, GLuint kVertices, const GLuint* pIndices, GLuint kIndices)
        {
            const GLuint kTris = kIndices / 3;

            // Tangents are accumulated so we need some space to work in.
            glm::vec3* buffer = new glm::vec3[kVertices * 2];
            memset(buffer, 0, sizeof(glm::vec3) * kVertices * 2);
            
            // offsets into the buffer;
            glm::vec3* tan1 = buffer;
            glm::vec3* tan2 = buffer + kVertices;

            // Step through each triangle.
            for (GLuint iTri = 0; iTri < kTris; ++iTri)
            {
                GLuint i1 = pIndices[0];
                GLuint i2 = pIndices[1];
                GLuint i3 = pIndices[2];

                glm::vec3 p1 = pVertices[i1].pos;
                glm::vec3 p2 = pVertices[i2].pos;
                glm::vec3 p3 = pVertices[i3].pos;

                glm::vec2 w1 = pVertices[i1].tex;
                glm::vec2 w2 = pVertices[i2].tex;
                glm::vec2 w3 = pVertices[i3].tex;

                GLfloat x1 = p2.x - p1.x;
                GLfloat x2 = p3.x - p1.x;
                GLfloat y1 = p2.y - p1.y;
                GLfloat y2 = p3.y - p1.y;
                GLfloat z1 = p2.z - p1.z;
                GLfloat z2 = p3.z - p1.z;

                GLfloat s1 = w2.x - w1.x;
                GLfloat s2 = w3.x - w1.x;
                GLfloat t1 = w2.y - w1.y;
                GLfloat t2 = w3.y - w1.y;

                GLfloat r = 1.f / (s1 * t2 - s2 * t1);
                glm::vec3 sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r, (t2 * z1 - t1 * z2) * r);
                glm::vec3 tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r, (s1 * z2 - s2 * z1) * r);

                // accumulate the tangents
                tan1[i1] += sdir;
                tan1[i2] += sdir;
                tan1[i3] += sdir;

                tan2[i1] += tdir;
                tan2[i2] += tdir;
                tan2[i3] += tdir;

                pIndices += 3;
            }

            // Step through each vertex.
            for (GLuint i = 0; i < kVertices; ++i)
            {
                glm::vec3 n = pVertices[i].normal;
                glm::vec3 t1 = tan1[i];
                glm::vec3 t2 = tan2[i];
                
                // Gram-Schmidt Orthogonalization
                glm::vec3 tangent = glm::normalize(t1 - n * glm::dot(n, t1));
                glm::vec3 bitangent = glm::cross(n, t1);

                //XMStoreFloat4(pVertices[i].tangent, tangent);
                pVertices[i].tangent = tangent;
                //pVertices[i].tangent.w = bitangent.x < 0.f ? -1.0f : 1.0f; // sign
                pVertices[i].bitangent = bitangent;
            }

            // cleanup the temp buffer
            delete[] buffer;
        }

        lithium::Object* processMesh(lithium::Model* model, aiMesh* mesh, const aiScene* scene, lithium::Mesh::State state)
        {
            //vector<Vertex> vertices;
            std::vector<unsigned int> indices;
            std::vector<lithium::ImageTexture*> textures;

            bool computeTangents = (state == lithium::Mesh::State::POS_NORMAL_UV_TANGENTS_BONE_WEIGHT);

            std::vector<MeshVertex> meshVertices;

            for (unsigned int i = 0; i < mesh->mNumVertices; i++)
            {
                auto p = AssimpGLMHelpers::GetGLMVec(mesh->mVertices[i]);
                auto n = AssimpGLMHelpers::GetGLMVec(mesh->mNormals[i]);
                glm::vec2 t;
                
                if (mesh->mTextureCoords[0])
                {
                    t.x = mesh->mTextureCoords[0][i].x;
                    t.y = mesh->mTextureCoords[0][i].y;
                }
                else
                {
                    t = glm::vec2(0.0f, 0.0f);
                }
                meshVertices.push_back(MeshVertex{p, n, t, glm::vec3{0.0f}, glm::vec3{0.0f}, {-1.0f, -1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 0.0f, 0.0f}});
            }
            for (unsigned int i = 0; i < mesh->mNumFaces; i++)
            {
                aiFace face = mesh->mFaces[i];
                for (unsigned int j = 0; j < face.mNumIndices; j++)
                    indices.push_back(face.mIndices[j]);
            }
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

            std::vector<lithium::ImageTexture*> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "u_texture", GL_SRGB, GL_RGBA);
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

            std::vector<lithium::ImageTexture*> normalMaps;
            for(auto texture : textures)
            {
                auto texName = texture->path();
                auto normalName = texName.substr(0, texName.find_last_of("."))
                    + "-nmap"
                    + texName.substr(texName.find_last_of("."), std::string::npos);
                std::ifstream f{normalName};
                if(f.good())
                {
                    normalMaps.push_back(lithium::ImageTexture::load(normalName, GL_RGB, GL_RGBA, GL_LINEAR, GL_REPEAT, GL_TEXTURE2));
                }
            }

            ExtractBoneWeightForVertices(model, meshVertices, mesh, scene);

            if(computeTangents)
            {
                compute_tangents_lengyel(&meshVertices[0], meshVertices.size(), &indices[0], indices.size());
            }

            std::vector<GLfloat> vertices;
            if(computeTangents)
            {
                vertices.reserve(meshVertices.size() * sizeof(MeshVertex));
            }
            else
            {
                vertices.reserve(meshVertices.size() * 16 * sizeof(float));
            }
#ifdef MODELLOADER_VERTEX_LOG
            _vertexLog << std::fixed << std::setprecision(2);
#endif
            for(MeshVertex meshVertex : meshVertices)
            {
#ifdef MODELLOADER_VERTEX_LOG
                _vertexLog << meshVertex.pos.x << " " << meshVertex.pos.y << " " << meshVertex.pos.z << " " << meshVertex.normal.x << " " << meshVertex.normal.y << " " << meshVertex.normal.z << " " <<
                    meshVertex.tex.x << " " << meshVertex.tex.y << " " << meshVertex.tangent.x << " " << meshVertex.tangent.y << " " << meshVertex.tangent.z << " " <<
                    meshVertex.bitangent.x << " " << meshVertex.bitangent.y << " " << meshVertex.bitangent.z << " " <<
                    meshVertex.boneIds[0] << " " << meshVertex.boneIds[1] << " " << meshVertex.boneIds[2] << " " << meshVertex.boneIds[3] << " " <<
                    meshVertex.boneWeights[0] << " " << meshVertex.boneWeights[1] << " " << meshVertex.boneWeights[2] << " " << meshVertex.boneWeights[3] << std::endl;
#endif
                if(computeTangents)
                {
                    vertices.insert(vertices.end(), {meshVertex.pos.x, meshVertex.pos.y, meshVertex.pos.z, meshVertex.normal.x, meshVertex.normal.y, meshVertex.normal.z,
                        meshVertex.tex.x, meshVertex.tex.y, meshVertex.tangent.x, meshVertex.tangent.y, meshVertex.tangent.z,
                        meshVertex.bitangent.x, meshVertex.bitangent.y, meshVertex.bitangent.z,
                        meshVertex.boneIds[0], meshVertex.boneIds[1], meshVertex.boneIds[2], meshVertex.boneIds[3],
                        meshVertex.boneWeights[0], meshVertex.boneWeights[1], meshVertex.boneWeights[2], meshVertex.boneWeights[3]});
                }
                else
                {
                    vertices.insert(vertices.end(), {meshVertex.pos.x, meshVertex.pos.y, meshVertex.pos.z, meshVertex.normal.x, meshVertex.normal.y, meshVertex.normal.z,
                        meshVertex.tex.x, meshVertex.tex.y,
                        meshVertex.boneIds[0], meshVertex.boneIds[1], meshVertex.boneIds[2], meshVertex.boneIds[3],
                        meshVertex.boneWeights[0], meshVertex.boneWeights[1], meshVertex.boneWeights[2], meshVertex.boneWeights[3]});
                }
            }

#ifdef MODELLOADER_VERTEX_LOG
            _vertexLog.flush();
            _vertexLog.close();
#endif

            auto lithiummesh = new lithium::Mesh(vertices, indices, state);

            auto object = new lithium::Object(lithiummesh, diffuseMaps.size() > 0 ? diffuseMaps[0] : nullptr, normalMaps.size() > 0 ? normalMaps[0] : nullptr);

            return object;
        }

        void ExtractBoneWeightForVertices(lithium::Model* model, std::vector<MeshVertex>& vertices, aiMesh* mesh, const aiScene* scene)
        {

            for (int boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
            {
                int boneID = -1;
                std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
                if (model->m_BoneInfoMap.find(boneName) == model->m_BoneInfoMap.end())
                {
                    BoneInfo newBoneInfo;
                    newBoneInfo.id = model->m_BoneInfoMap.size();
                    newBoneInfo.offset = AssimpGLMHelpers::ConvertMatrixToGLMFormat(mesh->mBones[boneIndex]->mOffsetMatrix);
                    model->m_BoneInfoMap[boneName] = newBoneInfo;
                    boneID = model->m_BoneInfoMap[boneName].id;
                }
                else
                {
                    boneID = model->m_BoneInfoMap[boneName].id;
                }
                assert(boneID != -1);
                auto weights = mesh->mBones[boneIndex]->mWeights;
                int numWeights = mesh->mBones[boneIndex]->mNumWeights;

                for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
                {
                    int vertexId = weights[weightIndex].mVertexId;
                    float weight = weights[weightIndex].mWeight;
                    assert(vertexId <= vertices.size());
                    for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
                    {
                        if (vertices[vertexId].boneIds[i] < 0)
                        {
                            vertices[vertexId].boneWeights[i] = weight;
                            vertices[vertexId].boneIds[i] = boneID;
                            break;
                        }
                    }
                }
            }
        }

        // checks all material textures of a given type and loads the textures if they're not loaded yet.
        // the required info is returned as a Texture struct.
        std::vector<lithium::ImageTexture*> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName, GLenum internalFormat=GL_RGBA,
            GLenum colorFormat=GL_RGBA, GLuint filter=GL_LINEAR, GLuint textureWrap=GL_CLAMP_TO_EDGE, GLuint textureUnit=GL_TEXTURE0)
        {
            std::vector<lithium::ImageTexture*> textures;
            for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
            {
                aiString str;
                mat->GetTexture(type, i, &str);
                auto path = _directory / str.C_Str();
                lithium::ImageTexture* texture = lithium::ImageTexture::load(path.u8string(), internalFormat, colorFormat, filter, textureWrap, textureUnit);
                textures.push_back(texture);
            }
            return textures;
        }

        std::map<std::string, lithium::Model*> _models;
        std::filesystem::path _directory;
#ifdef MODELLOADER_VERTEX_LOG
        std::ofstream _vertexLog;
#endif
    };
}