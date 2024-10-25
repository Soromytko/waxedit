#pragma once
#include "../ShaderProgram.h"
#include <glad/glad.h>

class OpenGLShaderProgram : public ShaderProgram
{
public:
	OpenGLShaderProgram(std::string&& vertexSrc, std::string&& fragmentSrc);
	~OpenGLShaderProgram();
	bool compile(InfoLog* result) const override;

private:
	GLuint _vertexShaderId{};
	GLuint _fragmentShaderId{};
};