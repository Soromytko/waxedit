#include<iostream>
#include "App.h"
#include "Window.h"
#include "Renderer/Renderer.h"

App::App()
{
	if (!tryCreateMainWindow())
	{
		std::cout << "Failed to create main window" << std::endl;
		_result = -1;
		return;
	}
	if (!initGraphicalApi())
	{
		std::cout << "Failed to initialize graphical api" << std::endl;
		_result = -1;
		return;
	}
}

int App::run()
{
	if (_result != 0)
	{
		return _result;
	}

	while (_mainWindow->isOpen())
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(1, 0, 0, 1);

		_mainWindow->swapBuffers();
		_mainWindow->processEvents();
	}
	return _result;
}

bool App::tryCreateMainWindow()
{
	_mainWindow.reset(new Window(600, 400, "Waxedit"));
	if (Window::isInitialized())
	{
		_mainWindow->makeContextCurrent();
		return true;
	}
	return false;
}

bool App::initGraphicalApi()
{
	Renderer::setApi(Renderer::Api::OpenGL);
	return Renderer::init();
}
