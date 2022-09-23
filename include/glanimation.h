#pragma once

#include <vector>
#include <map>
#include <glm/glm.hpp>
#include <assimp/scene.h>
#include "glbone.h"
#include <functional>
#include "glanimdata.h"

namespace lithium
{
	struct AssimpNodeData
	{
		glm::mat4 transformation;
		std::string name;
		int childrenCount;
		std::vector<AssimpNodeData> children;
	};

	class Animation
	{
	public:
		Animation() = default;

		Animation(const std::string& animationPath, std::map<std::string, BoneInfo>& boneInfoMap, size_t index)
		{
			Assimp::Importer importer;
			const aiScene* scene = importer.ReadFile(animationPath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace );
			assert(scene && scene->mRootNode);
			auto animation = scene->mAnimations[index];
			std::cout << animationPath << std::endl;
			for(int i = 0; i < scene->mNumAnimations; ++i)
			{
				std::cout << scene->mAnimations[i]->mName.C_Str() << " | ";
			}
			std::cout << std::endl;
			std::cout << animation->mName.C_Str() << " " << animation->mDuration << " " << animation->mTicksPerSecond << " "
				<< scene->mRootNode->mTransformation.a1 << "," << scene->mRootNode->mTransformation.a2 << ","
				<< scene->mRootNode->mTransformation.a3 << "," << scene->mRootNode->mTransformation.a4 << std::endl
				<< scene->mRootNode->mTransformation.b1 << "," << scene->mRootNode->mTransformation.b2 << ","
				<< scene->mRootNode->mTransformation.b3 << "," << scene->mRootNode->mTransformation.b4 << std::endl
				<< scene->mRootNode->mTransformation.c1 << "," << scene->mRootNode->mTransformation.c2 << ","
				<< scene->mRootNode->mTransformation.c3 << "," << scene->mRootNode->mTransformation.c4 << std::endl
				<< scene->mRootNode->mTransformation.d1 << "," << scene->mRootNode->mTransformation.d2 << ","
				<< scene->mRootNode->mTransformation.d3 << "," << scene->mRootNode->mTransformation.d4 << std::endl;
			m_Duration = animation->mDuration;
			m_TicksPerSecond = animation->mTicksPerSecond;
			aiMatrix4x4 globalTransformation = scene->mRootNode->mTransformation;
			/*globalTransformation.a1 = 1;
			globalTransformation.a2 = 0;
			globalTransformation.a3 = 0;
			globalTransformation.a4 = 0;
			globalTransformation.b1 = 0;
			globalTransformation.b2 = 0;
			globalTransformation.b3 = 1;
			globalTransformation.b4 = 0;
			globalTransformation.c1 = 0;
			globalTransformation.c2 = -1;
			globalTransformation.c3 = 0;
			globalTransformation.c4 = 0;
			globalTransformation.d1 = 0;
			globalTransformation.d2 = 0;
			globalTransformation.d3 = 0;
			globalTransformation.d4 = 1;*/
			globalTransformation = globalTransformation.Inverse();
			ReadHeirarchyData(m_RootNode, scene->mRootNode);
			ReadMissingBones(animation, boneInfoMap);
		}

		~Animation()
		{
		}

		Bone* FindBone(const std::string& name)
		{
			auto iter = std::find_if(m_Bones.begin(), m_Bones.end(),
				[&](const Bone& Bone)
				{
					return Bone.GetBoneName() == name;
				}
			);
			if (iter == m_Bones.end()) return nullptr;
			else return &(*iter);
		}

		
		inline float GetTicksPerSecond() { return m_TicksPerSecond; }
		inline float GetDuration() { return m_Duration;}
		inline const AssimpNodeData& GetRootNode() { return m_RootNode; }
		inline const std::map<std::string,BoneInfo>& GetBoneIDMap() 
		{ 
			return m_BoneInfoMap;
		}

	private:
		void ReadMissingBones(const aiAnimation* animation, std::map<std::string, BoneInfo>& boneInfoMap)
		{
			int size = animation->mNumChannels;

			//reading channels(bones engaged in an animation and their keyframes)
			for (int i = 0; i < size; i++)
			{
				auto channel = animation->mChannels[i];
				std::string boneName = channel->mNodeName.data;

				if (boneInfoMap.find(boneName) == boneInfoMap.end())
				{
					BoneInfo boneInfo{};
					boneInfo.id = boneInfoMap.size();
					boneInfoMap.emplace(boneName, boneInfo);
					std::cout << "adding bone: " << boneInfo.id << std::endl;
				}
				m_Bones.push_back(Bone(channel->mNodeName.data,
					boneInfoMap[channel->mNodeName.data].id, channel));
			}

			m_BoneInfoMap = boneInfoMap;
		}

		void ReadHeirarchyData(AssimpNodeData& dest, const aiNode* src)
		{
			assert(src);

			dest.name = src->mName.data;
			dest.transformation = AssimpGLMHelpers::ConvertMatrixToGLMFormat(src->mTransformation);
			dest.childrenCount = src->mNumChildren;

			for (int i = 0; i < src->mNumChildren; i++)
			{
				AssimpNodeData newData;
				ReadHeirarchyData(newData, src->mChildren[i]);
				dest.children.push_back(newData);
			}
		}
		float m_Duration;
		int m_TicksPerSecond;
		std::vector<Bone> m_Bones;
		AssimpNodeData m_RootNode;
		std::map<std::string, BoneInfo> m_BoneInfoMap;
	};
}