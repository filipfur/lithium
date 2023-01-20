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
                : Animation2{name, numberOfFrames}, _translationsMap{translationsMap}, _rotationsMap{rotationsMap},
                _animOfs{"anim.ofs"}
            {
                _lastFrame = std::chrono::steady_clock::now();
            }

            SkinAnimation(const SkinAnimation& other) : Animation2{other}
            {

            }

            virtual ~SkinAnimation() noexcept
            {
                _animOfs.flush();
                _animOfs.close();
            }

            virtual void onAnimationFrameChanged(int frame)
            {
#ifdef RECORD_ANIM
                std::chrono::steady_clock::time_point n = std::chrono::steady_clock::now();
                long duration = std::chrono::duration_cast<std::chrono::milliseconds>(n - _lastFrame).count();
                _lastFrame = n;
                _animOfs << _counter++ << ' ' << duration << std::endl;
#endif
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
            std::ofstream _animOfs;
            std::chrono::steady_clock::time_point _lastFrame;
            int _counter{0};
    };
}