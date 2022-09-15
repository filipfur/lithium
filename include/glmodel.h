#pragma once 

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include "glassimp_glm_helpers.h"
#include "glanimdata.h"
#include "glanimator.h"
#include "glmesh.h"
#include "iupdateable.h"

#define MAX_BONE_INFLUENCE 4
#define dirname(path) path.substr(0, path.find_last_of("/"))

namespace lithium
{
    class Animation;

    class Model : public IUpdateable
    {
    public:
        Model(std::string const &path, lithium::ShaderProgram* shaderProgram) : _shaderProgram{shaderProgram}
        {
            shaderProgram->use();
            shaderProgram->setUniform("u_texture_0", 0);
            shaderProgram->setUniform("u_specular_0", 1);
            shaderProgram->setUniform("u_normal_0", 2);
            loadModel(path);
        }

        void loadAnimation(const std::string path)
        {
            auto animation = new lithium::Animation(path, m_BoneInfoMap, m_BoneCounter);
            _animations.push_back(animation);
            _animator.playAnimation(animation);
        }

        void setShaderProgram(lithium::ShaderProgram* shaderProgram)
        {
            _shaderProgram = shaderProgram;
        }

        virtual void update(float dt) override
        {
            _animator.UpdateAnimation(dt);
            for(auto obj : _objects)
            {
                obj->update(dt);
            }
        }

        void render()
        {
            _shaderProgram->use();
            _animator.animate(_shaderProgram);
            for(auto obj : _objects)
            {
                obj->shade(_shaderProgram);
                obj->draw();
            }
        }
        
        auto& GetBoneInfoMap() { return m_BoneInfoMap; }
        int& GetBoneCount() { return m_BoneCounter; }

        lithium::Object* object()
        {
            return _objects.size() == 0 ? nullptr : _objects.at(0);
        }

        lithium::Object* object(size_t index)
        {
            lithium::Object* object{nullptr};
            if(index < _objects.size())
            {
                object = _objects.at(index);
            }
            return object;
        }
        

    private:

        std::vector<lithium::Texture*> textures_loaded;
        std::vector<lithium::Object*> _objects;
        lithium::ShaderProgram* _shaderProgram{nullptr};
        std::string _directory;
        lithium::Animator _animator;
        std::vector<lithium::Animation*> _animations;
        std::map<std::string, BoneInfo> m_BoneInfoMap;
        int m_BoneCounter = 0;

        // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
        void loadModel(std::string const &path)
        {
            // read file via ASSIMP
            Assimp::Importer importer;
            const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);
            // check for errors
            if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
            {
                std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
                return;
            }
            // retrieve the directory path of the filepath
            _directory = dirname(path);

            // process ASSIMP's root node recursively
            processNode(scene->mRootNode, scene);
        }

        // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
        void processNode(aiNode *node, const aiScene *scene)
        {
            // process each mesh located at the current node
            for(unsigned int i = 0; i < node->mNumMeshes; i++)
            {
                // the node object only contains indices to index the actual objects in the scene. 
                // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
                aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
                _objects.push_back(processMesh(mesh, scene));
            }
            // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
            for(unsigned int i = 0; i < node->mNumChildren; i++)
            {
                processNode(node->mChildren[i], scene);
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

        lithium::Object* processMesh(aiMesh* mesh, const aiScene* scene)
        {
            //vector<Vertex> vertices;
            std::vector<unsigned int> indices;
            std::vector<lithium::Texture*> textures;

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
                    t = glm::vec2(0.0f, 0.0f);

                meshVertices.push_back(MeshVertex{p,
                    n, t, glm::vec3{0.0f}, glm::vec3{0.0f}, {-1.0f, -1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 0.0f, 0.0f}});
            }
            for (unsigned int i = 0; i < mesh->mNumFaces; i++)
            {
                aiFace face = mesh->mFaces[i];
                for (unsigned int j = 0; j < face.mNumIndices; j++)
                    indices.push_back(face.mIndices[j]);
            }
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

            std::vector<lithium::Texture*> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "u_texture", GL_SRGB, GL_RGBA);
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

            std::vector<lithium::Texture*> normalMaps;
            for(auto texture : textures)
            {
                auto texName = texture->name();
                auto normalName = texName.substr(0, texName.find_last_of("."))
                    + "-nmap"
                    + texName.substr(texName.find_last_of("."), std::string::npos);
                std::cout << "d " << normalName << std::endl;
                std::ifstream f{normalName};
                if(f.good())
                {
                    normalMaps.push_back(new lithium::Texture(normalName, GL_RGB, GL_RGBA, GL_LINEAR, GL_REPEAT, GL_TEXTURE2));
                }
            }

            ExtractBoneWeightForVertices(meshVertices, mesh, scene);

            compute_tangents_lengyel(&meshVertices[0], meshVertices.size(), &indices[0], indices.size());

            std::vector<GLfloat> vertices;
            std::cout << "sizeof(MeshVertex): " << sizeof(MeshVertex) << std::endl;
            vertices.reserve(meshVertices.size() * sizeof(MeshVertex));
            for(MeshVertex meshVertex : meshVertices)
            {
                vertices.insert(vertices.end(), {meshVertex.pos.x, meshVertex.pos.y, meshVertex.pos.z, meshVertex.normal.x, meshVertex.normal.y, meshVertex.normal.z,
                    meshVertex.tex.x, meshVertex.tex.y, meshVertex.tangent.x, meshVertex.tangent.y, meshVertex.tangent.z,
                    meshVertex.bitangent.x, meshVertex.bitangent.y, meshVertex.bitangent.z,
                    meshVertex.boneIds[0], meshVertex.boneIds[1], meshVertex.boneIds[2], meshVertex.boneIds[3],
                    meshVertex.boneWeights[0], meshVertex.boneWeights[1], meshVertex.boneWeights[2], meshVertex.boneWeights[3]});
                    if(false)
                    {
                        std::cout << meshVertex.normal.x << " " << meshVertex.normal.y << " " << meshVertex.normal.z << std::endl;
                    }
            }

            auto lithiummesh = new lithium::Mesh(vertices, indices, lithium::Mesh::State::POS_NORMAL_UV_TANGENTS_BONE_WEIGHT);

            auto object = new lithium::Object(lithiummesh, diffuseMaps.size() > 0 ? diffuseMaps[0] : nullptr, normalMaps.size() > 0 ? normalMaps[0] : nullptr);

            return object;
        }

        void SetVertexBoneData(MeshVertex& vertex, int boneID, float weight)
        {
            for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
            {
                if (vertex.boneIds[i] < 0)
                {
                    vertex.boneWeights[i] = weight;
                    vertex.boneIds[i] = boneID;
                    break;
                }
            }
        }


        void ExtractBoneWeightForVertices(std::vector<MeshVertex>& vertices, aiMesh* mesh, const aiScene* scene)
        {
            auto& boneInfoMap = m_BoneInfoMap;
            int& boneCount = m_BoneCounter;

            for (int boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
            {
                int boneID = -1;
                std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
                if (boneInfoMap.find(boneName) == boneInfoMap.end())
                {
                    BoneInfo newBoneInfo;
                    newBoneInfo.id = boneCount;
                    newBoneInfo.offset = AssimpGLMHelpers::ConvertMatrixToGLMFormat(mesh->mBones[boneIndex]->mOffsetMatrix);
                    boneInfoMap[boneName] = newBoneInfo;
                    boneID = boneCount;
                    boneCount++;
                }
                else
                {
                    boneID = boneInfoMap[boneName].id;
                }
                assert(boneID != -1);
                auto weights = mesh->mBones[boneIndex]->mWeights;
                int numWeights = mesh->mBones[boneIndex]->mNumWeights;

                for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
                {
                    int vertexId = weights[weightIndex].mVertexId;
                    float weight = weights[weightIndex].mWeight;
                    assert(vertexId <= vertices.size());
                    SetVertexBoneData(vertices[vertexId], boneID, weight);
                }
            }
        }

        // checks all material textures of a given type and loads the textures if they're not loaded yet.
        // the required info is returned as a Texture struct.
        std::vector<lithium::Texture*> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName, GLenum internalFormat=GL_RGBA, GLenum colorFormat=GL_RGBA, GLuint filter=GL_LINEAR, GLuint textureWrap=GL_CLAMP_TO_EDGE, GLuint textureUnit=GL_TEXTURE0)
        {
            std::vector<lithium::Texture*> textures;
            for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
            {
                aiString str;
                mat->GetTexture(type, i, &str);
                // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
                bool skip = false;
                for(unsigned int j = 0; j < textures_loaded.size(); j++)
                {
                    if(std::strcmp(textures_loaded[j]->name().data(), str.C_Str()) == 0)
                    {
                        textures.push_back(textures_loaded[j]);
                        skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                        break;
                    }
                }
                if(!skip)
                {   // if texture hasn't been loaded already, load it
                    auto path = _directory + "/" + std::string(str.C_Str());
                    std::cout << path << std::endl;
                    lithium::Texture* texture = new lithium::Texture(path, internalFormat, colorFormat, filter, textureWrap, textureUnit);
                    textures.push_back(texture);
                    textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
                }
            }
            return textures;
        }
    };

}