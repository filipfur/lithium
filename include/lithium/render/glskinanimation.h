#pragma once

#include <vector>
#include <map>
#include <glm/glm.hpp>

#include "glanimation2.h"

namespace lithium
{
    class SkinAnimation : public Animation2
    {
        public:
            SkinAnimation(const std::string& name, int numberOfFrames,
                const std::map<int, std::vector<glm::vec3>>& translationsMap,
                const std::map<int, std::vector<glm::quat>>& rotationsMap)
                : Animation2{name, numberOfFrames}, _translationsMap{translationsMap}, _rotationsMap{rotationsMap}
            {
                
            }

            SkinAnimation(const SkinAnimation& other) : Animation2{other}
            {

            }

            virtual ~SkinAnimation() noexcept
            {

            }

            virtual void onAnimationFrameChanged(int frame)
            {

            }

            glm::vec3 getPosition(int index)
            {
                return _translationsMap[index][frame()];
            }

            glm::quat getRotation(int index)
            {
                return _rotationsMap[index][frame()];
            }

        private:
            std::map<int, std::vector<glm::vec3>> _translationsMap;
            std::map<int, std::vector<glm::quat>> _rotationsMap;
    };
}