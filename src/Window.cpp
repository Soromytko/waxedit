#include "Window.h"

Window::Window(int width, int height, const char* title)
{
	if (!init())
	{
		return;
	}
	_glfwWindow = glfwCreateWindow(width, height, title, NULL, NULL);
	if (!_glfwWindow)
	{
		return;
	}
	_windowCount++;
}

Window::~Window()
{
	if (_glfwWindow)
	{
		_windowCount--;
		if (_windowCount <= 0)
		{
			glfwTerminate();
			_glfwInitialized = false;
		}
	}
}

bool Window::isOpen()
{
	return !glfwWindowShouldClose(_glfwWindow);
}

void Window::makeContextCurrent()
{
	glfwMakeContextCurrent(_glfwWindow);
}

void Window::swapBuffers()
{
	glfwSwapBuffers(_glfwWindow);
}

void Window::processEvents()
{
	glfwPollEvents();
}

bool Window::isInitialized()
{
	return _glfwInitialized;
}

int Window::getWindowCount()
{
	return _windowCount;
}

bool Window::init()
{
	if (!_glfwInitialized)
	{
		_glfwInitialized = glfwInit();
	}
	return _glfwInitialized;
}

bool Window::_glfwInitialized = false;
int Window::_windowCount = 0;