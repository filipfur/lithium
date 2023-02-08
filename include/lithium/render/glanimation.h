#pragma once

#include <string>
#include "glianimation.h"

namespace lithium
{
    class Animation
    {
        public:
            Animation(const std::string& name, int numberOfFrames) : _name{name}, _numberOfFrames{numberOfFrames}
            {
                
            }

            Animation(const Animation& other)
                : _name{other._name}, _numberOfFrames{other._numberOfFrames},
                _iAnimation{other._iAnimation}, _looping{other._looping}, _playing{other._playing},
                _interval{other._interval}, _duration{other._duration}
            {

            }

            virtual ~Animation() noexcept
            {

            }

            virtual void onAnimationFrameChanged(int frame) = 0;

            void step(float dt)
            {
                int lastFrame{_frame};
                if(!_playing)
                {
                    return;
                }
                _duration -= dt;
                if(_duration <= 0)
                {
                    if(_reverse)
                    {
                        if(_frame == 0)
                        {
                            if(_looping)
                            {
                                _frame = _numberOfFrames - 1;
                            }
                            else
                            {
                                _playing = false;
                                if(_iAnimation)
                                {
                                    _iAnimation->onAnimationPlayedOnce(this);
                                }
                                return;
                            }
                        }
                        else
                        {
                            --_frame;
                        }
                    }
                    else
                    {
                        ++_frame;
                        if(_frame == _numberOfFrames)
                        {
                            _frame = _numberOfFrames - 1;
                            if(_looping)
                            {
                                _frame = 0;
                            }
                            else
                            {
                                _playing = false;
                                if(_iAnimation)
                                {
                                    _iAnimation->onAnimationPlayedOnce(this);
                                }
                                return;
                            }
                        }
                    }
                    if(lastFrame != _frame)
                    {
                        onAnimationFrameChanged(_frame);
                    }
                    //setMesh(*_currentFrame);
                    _duration = _interval + _duration;
                }
            }

            void play()
            {
                _playing = true;
                _duration = _interval;
                _frame = 0;
                onAnimationFrameChanged(_frame);
            }

            void setReverse(bool reverse)
            {
                _reverse = reverse;
            }

            void stop()
            {
                _playing = false;
            }

            void setIAnimation(lithium::IAnimation* iAnimation)
            {
                _iAnimation = iAnimation;
            }

            void setLooping(bool looping)
            {
                _looping = looping;
            }

            void setInterval(float interval)
            {
                _interval = interval;
            }

            bool playing() const
            {
                return _playing;
            }

            std::string name() const
            {
                return _name;
            }

            int frame() const
            {
                return _frame;
            }

            float progress() const
            {
                return 1.0f - _duration / _interval;
            }

        private:
            const std::string _name;
            const int _numberOfFrames;
            lithium::IAnimation* _iAnimation{nullptr};
            int _frame{0};
            float _interval{1.0f / 30.0f};
            float _duration{_interval};
            bool _looping{true};
            bool _playing{true};
            bool _reverse{false};
    };
}