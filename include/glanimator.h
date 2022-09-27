#pragma once

#include <glm/glm.hpp>
#include <map>
#include <vector>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include "glanimation.h"
#include "glbone.h"

namespace lithium
{
	class Animator
	{
	public:
		Animator()
		{
			m_FinalBoneMatrices.reserve(100);

			for (int i = 0; i < 100; i++)
				m_FinalBoneMatrices.push_back(glm::mat4(1.0f));			
		}

		void UpdateAnimation(float dt, const std::map<std::string, BoneInfo>& boneInfoMap)
		{
			if (m_CurrentAnimation)
			{
				m_CurrentTime += m_CurrentAnimation->GetTicksPerSecond() * dt;
				m_CurrentTime = fmod(m_CurrentTime, m_CurrentAnimation->GetDuration());
				CalculateBoneTransform(&m_CurrentAnimation->GetRootNode(), glm::mat4(1.0f), boneInfoMap);
			}
		}

		void playAnimation(Animation* pAnimation)
		{
			m_CurrentAnimation = pAnimation;
			m_CurrentTime = 0.0f;
		}

		void animate(lithium::ShaderProgram* shaderProgram)
		{
			auto transforms = GetFinalBoneMatrices();
			for (int i = 0; i < transforms.size(); ++i)
			{
				shaderProgram->setUniform("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);
			}
		}

		void CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform, const std::map<std::string, BoneInfo>& boneInfoMap)
		{
			std::string nodeName = node->name;
			glm::mat4 nodeTransform = node->transformation;

			Bone* Bone = m_CurrentAnimation->FindBone(nodeName);

			if (Bone)
			{
				Bone->Update(m_CurrentTime);
				nodeTransform = Bone->GetLocalTransform();
			}

			glm::mat4 globalTransformation = parentTransform * nodeTransform;

			auto it = boneInfoMap.find(nodeName);
			if (it != boneInfoMap.end())
			{
				int index = it->second.id;
				glm::mat4 offset = it->second.offset;
				m_FinalBoneMatrices[index] = globalTransformation * offset;
			}

			for (int i = 0; i < node->childrenCount; i++)
			{
				CalculateBoneTransform(&node->children[i], globalTransformation, boneInfoMap);
			}
		}

		std::vector<glm::mat4> GetFinalBoneMatrices()
		{
			return m_FinalBoneMatrices;
		}

	private:
		std::vector<glm::mat4> m_FinalBoneMatrices;
		Animation* m_CurrentAnimation{nullptr};
		float m_CurrentTime{0.0};
	};
}