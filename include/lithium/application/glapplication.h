#pragma once

#include <iostream>
#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "glupdateable.h"
#include "glinput.h"
#include "gluserpointer.h"

#ifdef LITHIUM_USE_AUDIO
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#endif

namespace lithium
{
    class Application : public lithium::Updateable, Input::Context
    {
    public:
        enum class Mode
        {
            DEFAULT,
            MULTISAMPLED_4X,
            MULTISAMPLED_8X
        };

        Application(const std::string& title, glm::ivec2 windowResolution, Mode mode=Mode::MULTISAMPLED_4X,
            bool fullscreen=false) : _windowResolution{windowResolution}, _mode(mode), _fullscreen{fullscreen}
        {
            glfwInit();
#ifdef __APPLE__
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#else
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
#endif
            glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_FALSE);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            //glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
            switch(mode)
            {
                case Mode::DEFAULT:
                    break;
                case Mode::MULTISAMPLED_4X:
                    glfwWindowHint(GLFW_SAMPLES, 4);
                    break;
                case Mode::MULTISAMPLED_8X:
                    glfwWindowHint(GLFW_SAMPLES, 8);
                    break;
            }

#ifdef LITHIUM_USE_AUDIO
            ma_result result = ma_engine_init(NULL, &_engine);
            if (result != MA_SUCCESS) {
                printf("Failed to initialize audio engine.");
                exit(-1);
            }
#endif

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
            _defaultFrameBufferResolution.x = fbWidth;
            _defaultFrameBufferResolution.y = fbHeight;

            _input = std::make_shared<lithium::Input>(_window);
            input()->setContext(this);

            glfwSetWindowSizeCallback(_window, windowSizeCallback);

            static UserPointer userPointer{this, _input.get()};
	        glfwSetWindowUserPointer(_window, &userPointer);
        }

        virtual ~Application()
        {
            glfwDestroyWindow(_window);
            glfwTerminate();
#ifdef LITHIUM_USE_AUDIO
            ma_engine_uninit(&_engine);	
#endif
        }

#ifdef LITHIUM_USE_AUDIO
        void playAudio(const std::string& path)
        {
            if(_audioEnabled)
            {
                ma_engine_set_volume(&_engine, 0.1f);
                ma_engine_play_sound(&_engine, path.c_str(), NULL);
            }
        }
#endif

        void run()
        {
            _lastTime = glfwGetTime();
            _startTime = _lastTime;
            _lastFpsCount = _lastTime;
            while (!glfwWindowShouldClose(_window))
            {
                // Simple timer
                double crntTime = glfwGetTime();
                _time = crntTime - _startTime;
                double dt = crntTime - _lastTime;
                // dt *= 0.1; // SLOMO (bad because lowers fps)
                if (dt > 1.0)
                {
                    std::cerr << "Too long since last tick. Discarding time." << std::endl;
                    _lastTime = crntTime;
                }
                else if (dt >= _maxFpsPeriod)
                {
                    update(dt);
                    _lastTime = crntTime;
                    //glfwSwapBuffers(_window);
                    glFlush();
                    ++_numFrames;
                }
                if(crntTime - _lastFpsCount >= 1)
                {
                    _fps = _numFrames;
                    onFpsCount(_fps);
                    _numFrames = 0;
                    _lastFpsCount = crntTime;
                }
                glfwPollEvents();
            }
        }

        float time() const
        {
            return _time;
        }

        virtual void onFpsCount(int fps)
        {

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

        glm::ivec2 windowResolution() const
        {
            return _windowResolution;
        }

        //virtual void update(float dt) = 0;

        void moveWindow(int x, int y)
        {
            glfwSetWindowPos(_window, x, y);
        }

        void setAudioEnabled(bool audioEnabled)
        {
            _audioEnabled = audioEnabled;
        }

        std::shared_ptr<lithium::Input> input() const
        {
            return _input;
        }

        int fps() const
        {
            return _fps;
        }

        GLFWwindow* window() const
        {
            return _window;
        }

        void close()
        {            
            glfwSetWindowShouldClose(_window, true);
        }

        void setMaxFps(float maxFps)
        {
            _maxFps = maxFps;
            _maxFpsPeriod = 1.0f / maxFps;
        }

        virtual void onWindowSizeChanged(int width, int height) = 0;

    private:
        static void windowSizeCallback(GLFWwindow* window, int width, int height)
        {
            Application* app = static_cast<lithium::UserPointer*>(glfwGetWindowUserPointer(window))->application;
            app->_windowResolution.x = width;
            app->_windowResolution.y = height;
            GLint dims[4] = {0};
            glGetIntegerv(GL_VIEWPORT, dims);
            GLint fbWidth = dims[2];
            GLint fbHeight = dims[3];
            app->_defaultFrameBufferResolution.x = fbWidth;
            app->_defaultFrameBufferResolution.y = fbHeight;
            app->onWindowSizeChanged(width, height);
        }

        std::shared_ptr<lithium::Input> _input{nullptr};
        glm::ivec2 _windowResolution;
        Mode _mode;
        GLFWwindow* _window{nullptr};
        float _time{0};
        double _lastTime{0};
        double _startTime{0};
        float _maxFps{240};
        float _maxFpsPeriod{1.0f / _maxFps};
        bool _fullscreen{false};
        glm::ivec2 _defaultFrameBufferResolution;
#ifdef LITHIUM_USE_AUDIO
        ma_engine _engine;
#endif
        int _fps{0};
        int _numFrames{0};
        double _lastFpsCount;
        bool _audioEnabled{true};
    };
}