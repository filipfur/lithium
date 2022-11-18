#pragma once

#include <vector>
#include <assimp/scene.h>
#include <list>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include "glassimp_glm_helpers.h"

namespace lithium
{
	class Bone
	{
	public:
		Bone(const std::string& name, int id, const aiNodeAnim* channel)
			:
			_name(name),
			_id(id),
			_localTransform(1.0f)
		{
			for (int positionIndex = 0; positionIndex < channel->mNumPositionKeys; ++positionIndex)
			{
				aiVector3D aiPosition = channel->mPositionKeys[positionIndex].mValue;
				float timeStamp = channel->mPositionKeys[positionIndex].mTime;
				_positions.push_back(AssimpGLMHelpers::GetGLMVec(aiPosition));
			}

			for (int rotationIndex = 0; rotationIndex < channel->mNumRotationKeys; ++rotationIndex)
			{
				aiQuaternion aiOrientation = channel->mRotationKeys[rotationIndex].mValue;
				float timeStamp = channel->mRotationKeys[rotationIndex].mTime;
				_rotations.push_back(AssimpGLMHelpers::GetGLMQuat(aiOrientation));
			}

			for (int keyIndex = 0; keyIndex < channel->mNumScalingKeys; ++keyIndex)
			{
				aiVector3D scale = channel->mScalingKeys[keyIndex].mValue;
				float timeStamp = channel->mScalingKeys[keyIndex].mTime;
				_scales.push_back(AssimpGLMHelpers::GetGLMVec(scale));
			}
		}
		
		void update(float animationTime)
		{
			// TODO: Optimize me.
			glm::mat4 translation = interpolatePosition(animationTime);
			glm::mat4 rotation = interpolateRotation(animationTime);
			glm::mat4 scale = interpolateScaling(animationTime);
			_localTransform = translation * rotation * scale;
		}

		glm::mat4 localTransform() { return _localTransform; }
		std::string name() const { return _name; }
		int id() { return _id; }
		
		int getIndex(float animationTime)
		{
			int index = (int)floorf(animationTime / _msPerFrame);
			if(index > _positions.size() - 1) {index = _positions.size() - 1;}
			if(index < 0) {index = 0;}
			return index;
		}

	private:

		glm::mat4 interpolatePosition(float animationTime)
		{
			if (_positions.size() == 1)
			{
				return glm::translate(glm::mat4(1.0f), _positions[0]);
			}
			int index = getIndex(animationTime);
			float time = index * _msPerFrame;
			float scaleFactor = (animationTime - time) / _msPerFrame;
			//float scaleFactor = (glm::modf(animationTime, _msPerFrame)) / _msPerFrame;
			return glm::translate(glm::mat4(1.0f),
				glm::mix(_positions[index],
					_positions[(index == _positions.size() - 1) ? 0 : index + 1],
					scaleFactor));
		}

		glm::mat4 interpolateRotation(float animationTime)
		{
			if (_rotations.size() == 1)
			{
				auto rotation = glm::normalize(_rotations[0]);
				return glm::toMat4(rotation);
			}
			int index = getIndex(animationTime);
			int nextIndex = (index == _positions.size() - 1) ? 0 : index + 1;
			float time = index * _msPerFrame;
			float scaleFactor = (animationTime - time) / _msPerFrame;
			return glm::toMat4(
				glm::normalize(glm::slerp(
					_rotations[index],
					_rotations[nextIndex],
					scaleFactor)));
		}

		glm::mat4 interpolateScaling(float animationTime)
		{
			if (_scales.size() == 1)
			{
				return glm::scale(glm::mat4(1.0f), _scales[0]);
			}
			int index = getIndex(animationTime);
			float time = index * _msPerFrame;
			float scaleFactor = (animationTime - time) / _msPerFrame;
			return glm::scale(glm::mat4(1.0f),
				glm::mix(
					_scales[index],
					_scales[(index == _positions.size() - 1) ? 0 : index + 1],
					scaleFactor));
		}

		std::vector<glm::vec3> _positions; // TODO: Maybe store as matrix instead? For optimize.
		std::vector<glm::quat> _rotations;
		std::vector<glm::vec3> _scales;

		static constexpr float _framePerSecond{30.0f};
		static constexpr float _msPerFrame{1000.0f / _framePerSecond};

		glm::mat4 _localTransform;
		std::string _name;
		int _id;
	};
}