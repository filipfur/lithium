#include "glinput.h"
#include <iostream>

lithium::Input::Input(GLFWwindow* window) : _window{ window }, _controller{}, _up{0.0f, 1.0f, 0.0f}
{
	glfwSetWindowUserPointer(window, this);
	glfwSetKeyCallback(window, onKeyStatic);
	glfwSetMouseButtonCallback(window, onMouseStatic);
	glfwSetScrollCallback(window, onScrollStatic);
	glfwSetCursorPosCallback(window, onCursorStatic);
	glfwSetCharCallback(window, onTextStatic);
	glfwGetWindowSize(window, &_width, &_height);
}

lithium::Input::~Input() noexcept
{

}

void lithium::Input::onKey(GLFWwindow *window, int key, int scanCode, int action, int mods)
{
	float speed = 0.0f;
	glm::vec3 delta = glm::vec3(0.0f);
	if(_anyKeyPressedCallback && key == GLFW_KEY_ENTER && action == GLFW_PRESS)
	{
		if(_anyKeyPressedCallback(key, mods))
		{
			return;
		}
	}

	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		delta.z = -1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		delta.x = -1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		delta.z = 1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		delta.x = 1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		delta.y = 1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		delta.y = -1.0f;
	}
	_controller.setDelta(delta);
	if(_context != nullptr)
	{
		if(action == GLFW_PRESS)
		{
			auto cacheIt = _keyCaches.find(_context);
			if(cacheIt != _keyCaches.end())
			{
				if(cacheIt->second->isCached(key))
				{
					cacheIt->second->setPressed(key);
				}
			}
			auto it = _pressedCallbacks.find(_context);
			if(it != _pressedCallbacks.end())
			{
				auto itPress = it->second.find(key);
				if(itPress != it->second.end())
				{
					itPress->second(key, mods);
				}
			}
		}
		else if(action == GLFW_RELEASE)
		{
			auto cacheIt = _keyCaches.find(_context);
			if(cacheIt != _keyCaches.end())
			{
				if(cacheIt->second->isCached(key))
				{
					cacheIt->second->setReleased(key);
				}
			}
			auto it = _releasedCallbacks.find(_context);
			if(it != _releasedCallbacks.end())
			{
				auto itRelease = it->second.find(key);
				if(itRelease != it->second.end())
				{
					itRelease->second(key, mods);
				}
			}
		}
	}
}

void lithium::Input::onMouse(GLFWwindow *window, int button, int action, int mods)
{
	//TODO: DUPLICATED CODE check just above.
	if(_context != nullptr)
	{
		if(action == GLFW_PRESS)
		{
			auto it = _pressedCallbacks.find(_context);
			if(it != _pressedCallbacks.end())
			{
				auto itPress = it->second.find(button);
				if(itPress != it->second.end())
				{
					itPress->second(button, mods);
				}
			}
			_clickedPos = _mousePos;
		}
		else if(action == GLFW_RELEASE)
		{
			auto it = _releasedCallbacks.find(_context);
			if(it != _releasedCallbacks.end())
			{
				auto itRelease = it->second.find(button);
				if(itRelease != it->second.end())
				{
					itRelease->second(button, mods);
				}
			}
			//std::cout << "Dragged from: " << _clickedPos.x << "," << _clickedPos.y << " to " << _mousePos.x << "," << _mousePos.y << " (" << glm::distance(_clickedPos, _mousePos) << ")" << std::endl;
		}
	}
}

void lithium::Input::onScroll(GLFWwindow *window, double xoffset, double yoffset)
{
	if(_context)
	{
		auto it = _scrollCallbacks.find(_context);
		if(it != _scrollCallbacks.end())
		{
			it->second(xoffset, yoffset);
		}
	}
}

void lithium::Input::onCursor(GLFWwindow *window, double mouseX, double mouseY)
{
	if (_fpsControl)
	{
		if(_firstClick)
		{
			centerCursor();
			_firstClick = false;
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		}
	
		_mousePos.x = mouseX;
		_mousePos.y = mouseY;
		float rotX = 64.0f * (float)(mouseY - (_height / 2)) / _height;
		float rotY = 64.0f * (float)(mouseX - (_width / 2)) / _width;

		_controller.setOrientation(glm::rotate(_controller.orientation(), glm::radians(-rotX), glm::normalize(glm::cross(_controller.orientation(), _up))));
		_controller.setOrientation(glm::rotate(_controller.orientation(), glm::radians(-rotY), _up));

		centerCursor();
	}
	else
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL );
	}
	if(_context)
	{
		auto it = _cursorCallbacks.find(_context);
		if(it != _cursorCallbacks.end())
		{
			it->second(static_cast<float>(mouseX), static_cast<float>(mouseY));
		}
	}
}

void lithium::Input::onText(GLFWwindow *window, unsigned int codepoint)
{
	char c = codepoint;
	//std::cout << "lithium::Input::onText(): " << c << std::endl;
	if(_typewriteCallbacks[_context])
	{
		_typewriteCallbacks[_context](c);
	}
}