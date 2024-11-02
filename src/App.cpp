#include<iostream>
#include <GLFW/glfw3.h>
#include <rendell/rendell.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "App.h"
#include "Window.h"
#include <vector>
#include "FontRaster.h"

#define FONT_WIDTH 32.0f
#define FONT_HEIGHT 32.0f

struct Matrices
{
	glm::mat4 projectMat = glm::mat4(1.0);
	glm::mat4 viewMat = glm::mat4(1.0);
	glm::mat4 worldMat = glm::mat4(1.0);
};

App::App()
{
	if (!tryCreateMainWindow())
	{
		std::cout << "Failed to create main window" << std::endl;
		_result = -1;
		return;
	}
	if (!initRendell())
	{
		std::cout << "Failed to initialize graphical api" << std::endl;
		_result = -1;
		return;
	}
	if (!initTextRenderer())
	{
		std::cout << "Failed to initialize TextRenderer" << std::endl;
		_result = -1;
		return;
	}
	setupTextRenderer();
}

int App::run()
{
	if (_result != 0)
	{
		return _result;
	}

	rendell::setClearBits(rendell::colorBufferBit | rendell::depthBufferBit);

	const float color = 31.0 / 255;
	while (_mainWindow->isOpen())
	{
		rendell::clear();
		rendell::clearColor(color, color, color, 1);

		_textRenderer->draw(L"Hello World!");

		_mainWindow->swapBuffers();
		_mainWindow->processEvents();
	}

	rendell::release();

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

bool App::initRendell()
{
	rendell::Initer initer;
	initer.context = static_cast<void*>(glfwGetProcAddress);
	const bool result = rendell::init(initer);
	if (result)
	{
		rendell::setPixelUnpackAlignment(1);
	}
	return result;
}

bool App::initTextRenderer()
{
	FontRaster raster("../res/Fonts/mononoki/mononoki-Regular.ttf");
	if (!raster.isInitialized())
	{
		std::cout << "ERROR: FontRaster initializion failed" << std::endl;
		return false;
	}
	raster.setFontSize(FONT_WIDTH, FONT_HEIGHT);
	FontRasterizationResult fontRasterizationResult;
	if (!raster.rasterize(fontRasterizationResult))
	{
		std::cout << "ERROR: FontRaster rasterization failed" << std::endl;
		return false;
	}
	_textRenderer.reset(new TextRenderer(fontRasterizationResult, 600, 400));
	return _textRenderer->isInitialized();
}

void App::setupTextRenderer()
{
	Matrices matrices;
	matrices.projectMat = glm::ortho(0.0f, 600.0f, 0.0f, 400.0f, 0.1f, 100.0f);
	matrices.worldMat = glm::translate(glm::mat4(1.0), glm::vec3(10, 10, 0.0));
	glm::mat4 matrix = matrices.projectMat * matrices.viewMat * matrices.worldMat;
	_textRenderer->setMatrix(matrix);
	_textRenderer->setFontSize(glm::vec2(FONT_WIDTH, FONT_HEIGHT));
	_textRenderer->setColor(glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));
}
