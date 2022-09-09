#include "glinput.h"
#include <iostream>

mygl::Input::Input(GLFWwindow* window) : _window{ window }, _controller{}, _up{0.0f, 1.0f, 0.0f}
{
	glfwSetWindowUserPointer(window, this);
	glfwSetKeyCallback(window, onKeyStatic);
	glfwSetMouseButtonCallback(window, onMouseStatic);
	glfwSetScrollCallback(window, onScrollStatic);
	glfwSetCursorPosCallback(window, onCursorStatic);
	glfwSetCharCallback(window, onTextStatic);
	glfwGetWindowSize(window, &_width, &_height);
}

mygl::Input::~Input() noexcept
{

}

void mygl::Input::onKey(GLFWwindow *window, int key, int scanCode, int action, int mods)
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

	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		delta.z = -1.0f;
		//orientation = _orientation;
		//speed = _speed;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		delta.x = -1.0f;
		//orientation = -glm::normalize(glm::cross(_orientation, _up));
		//speed = _speed;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		delta.z = 1.0f;
		//orientation = -_orientation;
		//speed = _speed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		delta.x = 1.0f;
		//orientation = glm::normalize(glm::cross(_orientation, _up));
		//speed = _speed;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		delta.y = 1.0f;
		//orientation = _up;
		//speed = _speed;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		delta.y = -1.0f;
		//orientation = -_up;
		//speed = _speed;
	}
	_controller.setDelta(delta);
	if(_context != nullptr)
	{
		if(action == GLFW_PRESS)
		{
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
	//_position += speed * orientation;
}

void mygl::Input::onMouse(GLFWwindow *window, int button, int action, int mods)
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
		}
	}
}

void mygl::Input::onScroll(GLFWwindow *window, double xoffset, double yoffset)
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

void mygl::Input::onCursor(GLFWwindow *window, double mouseX, double mouseY)
{
		/*if (firstClick)
		{
			glfwSetCursorPos(window, _width / 2, _height / 2);
			firstClick = false;
		}*/
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	float rotX = 100.0f * (float)(mouseY - (_height / 2)) / _height;
	float rotY = 100.0f * (float)(mouseX - (_width / 2)) / _width;
	glm::vec3 newOrienation = glm::rotate(_controller.orientation(), glm::radians(-rotX),
			glm::normalize(glm::cross(_controller.orientation(), _up)));
	if (true || abs(glm::angle(newOrienation, _up) - glm::radians(90.0f)) <= glm::radians(85.0f))
	{
		_controller.setOrientation(newOrienation);
	}

	_controller.setOrientation(glm::rotate(_controller.orientation(), glm::radians(-rotY), _up));

	if(_context)
	{
		auto it = _cursorCallbacks.find(_context);
		if(it != _cursorCallbacks.end())
		{
			it->second(static_cast<float>(mouseX), static_cast<float>(mouseY));
		}
	}


	//glfwSetCursorPos(window, _width / 2, _height / 2);
}

void mygl::Input::onText(GLFWwindow *window, unsigned int codepoint)
{
	char c = codepoint;
	//std::cout << "mygl::Input::onText(): " << c << std::endl;
	if(_typewriteCallbacks[_context])
	{
		_typewriteCallbacks[_context](c);
	}
}