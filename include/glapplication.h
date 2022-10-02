#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "iupdateable.h"
#include "glinput.h"

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

namespace lithium
{
    class Application : public IUpdateable, Input::Context
    {
    public:
        enum class Mode
        {
            DEFAULT, MULTISAMPLED_4X
        };

        Application(const std::string& title, glm::ivec2 windowResolution, Mode mode=Mode::MULTISAMPLED_4X,
            bool fullscreen=false) : _windowResolution{windowResolution}, _mode(mode), _fullscreen{fullscreen}
        {
            glfwInit();
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            //glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
            switch(mode)
            {
                case Mode::DEFAULT:
                    break;
                case Mode::MULTISAMPLED_4X:
                    glfwWindowHint(GLFW_SAMPLES, 4);
                    break;
            }

            ma_result result = ma_engine_init(NULL, &_engine);
            if (result != MA_SUCCESS) {
                printf("Failed to initialize audio engine.");
                exit(-1);
            }

            GLFWmonitor *monitor = nullptr;
            if(_fullscreen)
            {
                monitor = glfwGetPrimaryMonitor();
            }

#ifdef __APPLE__
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

            _window = glfwCreateWindow(windowResolution.x, windowResolution.y, title.c_str(), monitor, nullptr);
            if (_window == nullptr)
            {
                std::cerr << "Failed to create a GLFW window" << std::endl;
                glfwTerminate();
                exit(-1);
            }
            glfwMakeContextCurrent(_window);

            gladLoadGL();

		    _lastTime = glfwGetTime();

            GLint dims[4] = {0};
            glGetIntegerv(GL_VIEWPORT, dims);
            GLint fbWidth = dims[2];
            GLint fbHeight = dims[3];
            std::cout << "fb: " << fbWidth << ", " << fbHeight << std::endl;
            _input = new lithium::Input(_window);
            _defaultFrameBufferResolution.x = fbWidth;
            _defaultFrameBufferResolution.y = fbHeight;
            input()->setContext(this);
        }

        virtual ~Application()
        {
            glfwDestroyWindow(_window);
            glfwTerminate();
            ma_engine_uninit(&_engine);	
        }

        void playAudio(const std::string& path)
        {
            if(_audioEnabled)
            {
                ma_engine_set_volume(&_engine, 0.1f);
                ma_engine_play_sound(&_engine, path.c_str(), NULL);
            }
        }

        void run()
        {
            _lastTime = glfwGetTime();
            _startTime = _lastTime;
            while (!glfwWindowShouldClose(_window))
            {
                // Simple timer
                double crntTime = glfwGetTime();
                _time = crntTime - _startTime;
                double dt = crntTime - _lastTime;
                // dt *= 0.1; SLOMO
                if (dt > 1.0)
                {
                    std::cerr << "Too long since last tick. Discarding time." << std::endl;
                    _lastTime = crntTime;
                }
                else if (dt >= 0.015f)
                {
                    update(dt);
                    _lastTime = crntTime;
                    glfwSwapBuffers(_window);
                }
                glfwPollEvents();
            }
        }

        float time() const
        {
            return _time;
        }

        glm::ivec2 defaultFrameBufferResolution() const
        {
            return _defaultFrameBufferResolution;
        }

        float windowWidth() const
        {
            return _windowResolution.x;
        }

        float windowHeight() const
        {
            return _windowResolution.y;
        }

        //virtual void update(float dt) = 0;

        void moveWindow(int x, int y)
        {
            glfwSetWindowPos(_window, x, y);
        }

        lithium::Input* input() const
        {
            return _input;
        }

    private:
        lithium::Input* _input{nullptr};
        glm::ivec2 _windowResolution;
        Mode _mode;
        GLFWwindow* _window{nullptr};
        float _time{0};
        double _lastTime{0};
        double _startTime{0};
        bool _fullscreen{false};
        glm::ivec2 _defaultFrameBufferResolution;
        ma_engine _engine;
        bool _audioEnabled{true};
    };
}