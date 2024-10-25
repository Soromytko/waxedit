#include<iostream>
#include "App.h"
#include "Window.h"
#include "Renderer/Renderer.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/VertexArray.h"
#include "Renderer/ShaderProgram.h"
#include <vector>

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

	IndexBuffer* indexBuffer = IndexBuffer::create({ 0, 1, 2 });
	VertexBuffer* vertexBuffer = VertexBuffer::create({
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f,
		});
	vertexBuffer->setLayouts({
		{ShaderDataType::float3, false, 0},
		});
	VertexArray* vertexArray = VertexArray::create();
	vertexArray->addVertexBuffer(vertexBuffer);
	vertexArray->setIndexBuffer(indexBuffer);

	std::string vertSrc = R"(
		#version 330 core
		layout (location = 0) in vec3 aPos;
		void main()
		{
		   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
		}
	)";

	std::string fragSrc = R"(
		out vec4 FragColor;
		void main()
		{
			FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
		}
	)";
	ShaderProgram* shaderProgram = ShaderProgram::create(vertSrc, fragSrc);
	std::string infoLog;
	if (!shaderProgram->compile(&infoLog))
	{
		std::cout << infoLog << std::endl;
		return -1;
	}
	if (!shaderProgram->link(&infoLog))
	{
		std::cout << infoLog << std::endl;
		return -1;
	}
	shaderProgram->freeSrc();

	const float color = 31.0 / 255;
	while (_mainWindow->isOpen())
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(color, color, color, 1);

		vertexArray->bind();

		shaderProgram->bind();
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

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
