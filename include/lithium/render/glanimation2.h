#pragma once

#include "glianimation2.h"

namespace lithium
{
    class Animation2
    {
        public:
            Animation2(const std::string& name, int numberOfFrames) : _name{name}, _numberOfFrames{numberOfFrames}
            {
                
            }

            Animation2(const Animation2& other)
                : _name{other._name}, _numberOfFrames{other._numberOfFrames},
                _iAnimation{other._iAnimation}, _looping{other._looping}, _playing{other._playing},
                _interval{other._interval}, _duration{other._duration}
            {

            }

            virtual ~Animation2() noexcept
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

            void setIAnimation(lithium::IAnimation2* iAnimation)
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

        private:
            const std::string _name;
            const int _numberOfFrames;
            lithium::IAnimation2* _iAnimation{nullptr};
            int _frame{0};
            float _interval{1.0f / 30.0f};
            float _duration{_interval};
            bool _looping{false};
            bool _playing{false};
            bool _reverse{false};
    };
}