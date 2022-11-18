
#pragma once

#include <functional>

namespace lithium
{
    class Updateable
    {
    public:
        virtual void update(float dt)
        {
            _time += dt;
            if(_updateCallback)
            {
                if(!_updateCallback(this, _time, dt))
                {
                    _updateCallback = [](lithium::Updateable*, float, float){ return true; };
                }
            }
        }

        void setUpdateCallback(const std::function<bool(Updateable*,float,float)>& updateCallback)
		{
			_updateCallback = updateCallback;
		}

        float time() const
        {
            return _time;
        }


    private:
		std::function<bool(lithium::Updateable*,float,float)> _updateCallback;
        float _time{0.0f};
    };
}