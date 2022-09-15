#pragma once

#include "iinput.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <map>
#include <functional>
#include <sstream>
#include <cassert>

namespace lithium
{
	class Input
	{
	public:

		class Context
		{

		};

		Input(GLFWwindow* window);
		virtual ~Input() noexcept;
		void update();

		void addIInput()
		{

		}

		static void onKeyStatic(GLFWwindow *window, int key, int scanCode, int action, int mods)
		{
			Input* self = static_cast<Input*>(glfwGetWindowUserPointer(window));
			self->onKey(window, key, scanCode, action, mods);
		}

		static void onMouseStatic(GLFWwindow *window, int button ,int action, int mods)
		{
			Input* self = static_cast<Input*>(glfwGetWindowUserPointer(window));
			self->onMouse(window, button, action, mods);
		}

		static void onScrollStatic(GLFWwindow *window, double xoffset, double yoffset)
		{
			Input* self = static_cast<Input*>(glfwGetWindowUserPointer(window));
			self->onScroll(window, xoffset, yoffset);
		}

		static void onCursorStatic(GLFWwindow *window, double xpos, double ypos)
		{
			Input* self = static_cast<Input*>(glfwGetWindowUserPointer(window));
			self->onCursor(window, xpos, ypos);
		}

		static void onTextStatic(GLFWwindow* window, unsigned int codepoint)
		{
			Input* self = static_cast<Input*>(glfwGetWindowUserPointer(window));
			self->onText(window, codepoint);
		}

		void onKey(GLFWwindow *window, int key, int scanCode, int action, int mods);

		void onMouse(GLFWwindow *window, int button, int action, int mods);

		void onScroll(GLFWwindow *window, double xoffset, double yoffset);

		void onCursor(GLFWwindow *window, double mouseX, double mouseY);

		void onText(GLFWwindow *window, unsigned int codepoint);

		void addPressedCallback(int key, const std::function<bool(int, int)>& func)
		{
			assert(_context != nullptr);
			_pressedCallbacks[_context][key] = func;
		}

		void addReleasedCallback(int key, const std::function<bool(int, int)>& func)
		{
			assert(_context != nullptr);
			_releasedCallbacks[_context][key] = func;
		}

		void setAnyKeyPressedListener(const std::function<bool(int, int)>& func)
		{
			_anyKeyPressedCallback = func;
		}

		void setScrollListener(const std::function<bool(float, float)>& func)
		{
			assert(_context != nullptr);
			_scrollCallbacks[_context] = func;
		}

		void setCursorListener(const std::function<bool(float, float)>& func)
		{
			assert(_context != nullptr);
			_cursorCallbacks[_context] = func;
		}

		void setTypewriteCallback(const std::function<bool(char)>& func)
		{
			assert(_context != nullptr);
			_typewriteCallbacks[_context] = func;
		}

		void centerCursor()
		{
			glfwSetCursorPos(_window, _width / 2, _height / 2);
		}

		class Controller
		{
		public:
			Controller() : _delta{0.0f}
			{

			}

			virtual ~Controller() noexcept
			{

			}

			const glm::vec3& delta() const
			{
				return _delta;
			}

			void setDelta(const glm::vec3& delta)
			{
				_delta = delta;
			}

			const glm::vec3& orientation() const
			{
				return _orientation;
			}

			void setOrientation(const glm::vec3& orientation)
			{
				_orientation = orientation;
			}

		private:
			glm::vec3 _delta;
			glm::vec3 _orientation;
		};

		Controller controller() const
		{
			return _controller;
		}

		void setContext(Context* context)
		{
			_context = context;
		}

	private:
		GLFWwindow* _window;
		Controller _controller;
		IInput* _iInput;
		int _width;
		int _height;
		Context* _context{nullptr};
		std::map<Context*,std::map<int, std::function<bool(int, int)>>> _pressedCallbacks;
		std::map<Context*,std::map<int, std::function<bool(int, int)>>> _releasedCallbacks;
		std::map<Context*, std::function<bool(float, float)>> _scrollCallbacks;
		std::map<Context*, std::function<bool(float, float)>> _cursorCallbacks;
		std::function<bool(int, int)> _anyKeyPressedCallback{nullptr};
		glm::vec3 _up;
		std::map<Context*, std::function<bool(char c)>> _typewriteCallbacks;
	};
}
