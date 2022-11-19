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
			const aiScene* scene = importer.ReadFile(animationPath, 0);//aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace );
			assert(scene && scene->mRootNode);
			auto animation = scene->mAnimations[index];
			/*std::cout << animationPath << std::endl;
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
				<< scene->mRootNode->mTransformation.d3 << "," << scene->mRootNode->mTransformation.d4 << std::endl;*/
			_duration = animation->mDuration;
			_ticksPerSecond = animation->mTicksPerSecond;
			aiMatrix4x4 globalTransformation = scene->mRootNode->mTransformation;
			globalTransformation = globalTransformation.Inverse();
			readHeirachyData(_rootNode, scene->mRootNode);
			readMissingBones(animation, boneInfoMap);
		}

		~Animation()
		{
		}

		Bone* findBone(const std::string& name)
		{
			auto iter = std::find_if(_bones.begin(), _bones.end(),
				[&](const Bone& bone)
				{
					return bone.name() == name;
				}
			);
			if (iter == _bones.end()) return nullptr;
			else return &(*iter);
		}

		void setLooped(bool looped)
		{
			_looped = looped;
		}

		bool looped() const
		{
			return _looped;
		}
		
		inline float ticksPerSecond() { return _ticksPerSecond; }
		inline float duration() { return _duration;}
		inline const AssimpNodeData& rootNode() { return _rootNode; }

	private:
		void readMissingBones(const aiAnimation* animation, std::map<std::string, BoneInfo>& boneInfoMap)
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
					//std::cout << "adding bone: " << boneInfo.id << std::endl;
				}
				_bones.push_back(Bone(channel->mNodeName.data,
					boneInfoMap[channel->mNodeName.data].id, channel));
			}
		}

		void readHeirachyData(AssimpNodeData& dest, const aiNode* src)
		{
			assert(src);

			dest.name = src->mName.data;
			dest.transformation = AssimpGLMHelpers::ConvertMatrixToGLMFormat(src->mTransformation);
			dest.childrenCount = src->mNumChildren;

			for (int i = 0; i < src->mNumChildren; i++)
			{
				AssimpNodeData newData;
				readHeirachyData(newData, src->mChildren[i]);
				dest.children.push_back(newData);
			}
		}
		float _duration;
		int _ticksPerSecond;
		bool _looped{true};
		std::vector<Bone> _bones;  // ?????
		AssimpNodeData _rootNode;
	};
}