#pragma once

#include <vector>
#include <map>
#include <glm/glm.hpp>

#include "glanimation.h"

namespace lithium
{
    class SkinAnimation : public Animation
    {
        public:
            SkinAnimation(const std::string& name, int numberOfFrames,
                const std::map<int, std::vector<glm::vec3>>& translationsMap,
                const std::map<int, std::vector<glm::quat>>& rotationsMap)
                : Animation{name, numberOfFrames}, _translationsMap{translationsMap}, _rotationsMap{rotationsMap},
                _animOfs{"anim.ofs"}
            {
                _lastFrame = std::chrono::steady_clock::now();
            }

            SkinAnimation(const SkinAnimation& other) : Animation{other}
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

            glm::vec3 lerp(const glm::vec3& a, const glm::vec3& b, float t)
            {
                return a * (1.f - t) + b * t;
            }

            glm::vec3 getPosition(int index)
            {
                if(!_interpolate)
                {
                    return _translationsMap[index][frame()];
                }
                const int j = frame();
                const int k = j == _translationsMap[index].size() - 1 ? 0 : j + 1;

                glm::vec3 p0 = _translationsMap[index][j];
                glm::vec3 p1 = _translationsMap[index][k];
                return lerp(p0, p1, progress());
            }

            glm::quat getRotation(int index)
            {
                if(!_interpolate)
                {
                    return _rotationsMap[index][frame()];
                }
                const int j = frame();
                const int k = j == _rotationsMap[index].size() - 1 ? 0 : j + 1;

                glm::quat p0 = _rotationsMap[index][j];
                glm::quat p1 = _rotationsMap[index][k];
                return glm::slerp(p0, p1, progress());
            }

            void setInterpolate(bool interpolate)
            {
                _interpolate = interpolate;
            }

            bool interpolate() const
            {
                return _interpolate;
            }

        private:
            std::map<int, std::vector<glm::vec3>> _translationsMap;
            std::map<int, std::vector<glm::quat>> _rotationsMap;
            std::ofstream _animOfs;
            std::chrono::steady_clock::time_point _lastFrame;
            int _counter{0};
            bool _interpolate{true};
    };
}