#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <unordered_map>
#include <memory>
#include <functional>
#include <sstream>
#include <cassert>
#include "gluserpointer.h"

namespace lithium
{
	class Input
	{
	public:

		enum class DragState
		{
			START,
			DRAG,
			END
		};

		class Context
		{

		};

		Input(GLFWwindow* window);
		virtual ~Input() noexcept;
		void update();

		void onKey(GLFWwindow *window, int key, int scanCode, int action, int mods);

		void onMouse(GLFWwindow *window, int button, int action, int mods);

		void onScroll(GLFWwindow *window, double xoffset, double yoffset);

		void onCursor(GLFWwindow *window, double mouseX, double mouseY);

		void onText(GLFWwindow *window, unsigned int codepoint);

		/*bool pressedCallback(int key, int mods)*/
		void addPressedCallback(int key, const std::function<bool(int, int)>& func)
		{
			assert(_context != nullptr);
			_pressedCallbacks[_context][key] = func;
		}

		/*bool releasedCallback(int key, int mods)*/
		void addReleasedCallback(int key, const std::function<bool(int, int)>& func)
		{
			assert(_context != nullptr);
			_releasedCallbacks[_context][key] = func;
		}

		/*
			button, modifiers, start, current, delta, dragState
		*/
		void setDragCallback(const std::function<void(int, int, const glm::vec2&, const glm::vec2&, const glm::vec2&, DragState)>& func)
		{
			assert(_context != nullptr);
			_dragCallbacks[_context] = func;
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

			float dx() const
			{
				return _delta.x;
			}

			float dy() const
			{
				return _delta.y;
			}

			float dz() const
			{
				return _delta.z;
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
			glm::vec3 _delta{0.0f};
			glm::vec3 _orientation{1.0f, 0.0f, 0.0f};
		};

		class KeyCache
		{
		public:
			KeyCache(std::initializer_list<int> keys)
			{
				for(auto key : keys)
				{
					_keys.emplace(key, false);
				}
			}

			bool isCached(int key)
			{
				auto it = _keys.find(key);
				return it != _keys.end();
			}

			void setPressed(int key)
			{
				_keys[key] = true;
			}

			void setReleased(int key)
			{
				_keys[key] = false;
			}

			bool isPressed(int key)
			{
				auto it = _keys.find(key);
				assert(it != _keys.end());
				return it->second;
			}

		private:
			std::unordered_map<int, bool> _keys;
		};

		void setKeyCache(std::shared_ptr<lithium::Input::KeyCache> keyCache)
		{
			assert(_context != nullptr);
			_keyCaches[_context] = keyCache;
		}

		Controller controller() const
		{
			return _controller;
		}

		void setContext(Context* context)
		{
			_context = context;
		}

		glm::vec2 mousePosition() const
		{
			return _mousePos;
		}

		glm::vec3 up() const
		{
			return _up;
		}

		void setFpsControl(bool fpsControl)
		{
			_fpsControl = fpsControl;
			_firstClick = true;
		}

		bool fpsControl() const
		{
			return _fpsControl;
		}

	private:
		static void onKeyStatic(GLFWwindow *window, int key, int scanCode, int action, int mods)
		{
			Input* self = static_cast<lithium::UserPointer*>(glfwGetWindowUserPointer(window))->input;
			self->onKey(window, key, scanCode, action, mods);
		}

		static void onMouseStatic(GLFWwindow *window, int button ,int action, int mods)
		{
			Input* self = static_cast<lithium::UserPointer*>(glfwGetWindowUserPointer(window))->input;
			self->onMouse(window, button, action, mods);
		}

		static void onScrollStatic(GLFWwindow *window, double xoffset, double yoffset)
		{
			Input* self = static_cast<lithium::UserPointer*>(glfwGetWindowUserPointer(window))->input;
			self->onScroll(window, xoffset, yoffset);
		}

		static void onCursorStatic(GLFWwindow *window, double xpos, double ypos)
		{
			Input* self = static_cast<lithium::UserPointer*>(glfwGetWindowUserPointer(window))->input;
			self->onCursor(window, xpos, ypos);
		}

		static void onTextStatic(GLFWwindow* window, unsigned int codepoint)
		{
			Input* self = static_cast<lithium::UserPointer*>(glfwGetWindowUserPointer(window))->input;
			self->onText(window, codepoint);
		}

		GLFWwindow* _window;
		Controller _controller;
		int _width;
		int _height;
		bool _fpsControl{false};
		Context* _context{nullptr};
		glm::vec2 _clickedPos;
		bool _firstClick{true};
		glm::vec2 _mousePos;
		glm::vec2 _lastDragPos;
		bool _dragging{false};
		int _dragButton;
		int _dragModifiers;
		std::unordered_map<Context*, std::shared_ptr<KeyCache>> _keyCaches;
		std::unordered_map<Context*,std::unordered_map<int, std::function<bool(int, int)>>> _pressedCallbacks;
		std::unordered_map<Context*,std::unordered_map<int, std::function<bool(int, int)>>> _releasedCallbacks;
		std::unordered_map<Context*, std::function<bool(float, float)>> _scrollCallbacks;
		std::unordered_map<Context*, std::function<bool(float, float)>> _cursorCallbacks;
		std::unordered_map<Context*, std::function<void(int, int, const glm::vec2&, const glm::vec2&, const glm::vec2&, DragState)>> _dragCallbacks;
		std::function<bool(int, int)> _anyKeyPressedCallback{nullptr};
		glm::vec3 _up{0.0f, 1.0f, 0.0f};
		std::unordered_map<Context*, std::function<bool(char c)>> _typewriteCallbacks;
	};

}
