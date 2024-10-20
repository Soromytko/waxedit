#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "Renderer.h"

Renderer::Api Renderer::getApi()
{
	return  _api;
}

void Renderer::setApi(Api api)
{
	_api = api;
}

bool Renderer::init(std::string *reason)
{
	char* reasonPtr = nullptr;
	if (!initApi())
	{
		reasonPtr = "Failed to initialize GLAD";
	} else if (!initFreeType)
	{
		reasonPtr = "ERROR::FREETYPE: Could not init FreeType Library";
	}

	if (reasonPtr)
	{
		if (reason)
		{
			*reason = reasonPtr;
		}
		return false;
	}
	return true;
}

bool Renderer::initApi()
{
	return gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
}

bool Renderer::initFreeType()
{
	// FreeType
	FT_Library ft;
	// All functions return a value different than 0 whenever an error occurred
	return FT_Init_FreeType(&ft);
}

Renderer::Api Renderer::_api{ Api::OpenGL };

