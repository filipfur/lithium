#pragma once

#include <glm/glm.hpp>

namespace lithium
{
	struct BoneInfo
	{
		/*id is index in finalBoneMatrices*/
		int id;

		/*offset matrix transforms vertex from model space to bone space*/
		glm::mat4 offset;

	};
}
#pragma once