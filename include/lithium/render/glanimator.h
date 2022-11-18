#pragma once

#include <glm/glm.hpp>
#include <map>
#include <vector>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include "glanimation.h"
#include "glbone.h"
#include "glshaderprogram.h"

namespace lithium
{
	class Animator
	{
	public:
		Animator()
		{
			_finalBoneMatrix.reserve(100);

			for (int i = 0; i < 100; i++)
				_finalBoneMatrix.push_back(glm::mat4(1.0f));			
		}

		void updateAnimation(float dt, const std::map<std::string, BoneInfo>& boneInfoMap)
		{
			if (_animation)
			{
				if(!_animation->looped() && _timeMs >= _animation->duration() - 66.66f) // TODO: Hack. Pls fix.
				{
					return;
				}
				_timeMs += _animation->ticksPerSecond() * dt;
				if(_timeMs > _animation->duration())
				{
					_timeMs = fmod(_timeMs, _animation->duration());
				}
				calculateBoneTransform(&_animation->rootNode(), glm::mat4(1.0f), boneInfoMap);
			}
		}

		void playAnimation(Animation* pAnimation)
		{
			_animation = pAnimation;
			_timeMs = 0.0f;
		}

		void animate(lithium::ShaderProgram* shaderProgram)
		{
			auto transforms = finalBoneMatrix();
			for (int i = 0; i < transforms.size(); ++i)
			{
				shaderProgram->setUniform("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);
			}
		}

		void calculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform, const std::map<std::string, BoneInfo>& boneInfoMap)
		{
			std::string nodeName = node->name;
			glm::mat4 nodeTransform = node->transformation;

			Bone* bone = _animation->findBone(nodeName);

			if (bone)
			{
				bone->update(_timeMs);
				nodeTransform = bone->localTransform();
			}

			glm::mat4 globalTransformation = parentTransform * nodeTransform;

			auto it = boneInfoMap.find(nodeName);
			if (it != boneInfoMap.end())
			{
				int index = it->second.id;
				glm::mat4 offset = it->second.offset;
				_finalBoneMatrix[index] = globalTransformation * offset;
			}

			for (int i = 0; i < node->childrenCount; i++)
			{
				calculateBoneTransform(&node->children[i], globalTransformation, boneInfoMap);
			}
		}

		std::vector<glm::mat4> finalBoneMatrix()
		{
			return _finalBoneMatrix;
		}

	private:
		std::vector<glm::mat4> _finalBoneMatrix;
		Animation* _animation{nullptr};
		float _timeMs{0.0};
	};
}