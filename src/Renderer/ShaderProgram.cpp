#include "ShaderProgram.h"
#include "Renderer.h"
#include "OpenGL/OpenGLShaderProgram.h"

ShaderProgram::ShaderProgram(std::string&& vertexSrc, std::string&& fragmentSrc) :
	_vertexSrc(std::move(vertexSrc)), _fragmentSrc(std::move(fragmentSrc))
{

}

ShaderProgram* ShaderProgram::create(const std::string& vertexSrc, const std::string& fragmentSrc)
{
	std::string vertex = vertexSrc;
	std::string fragment = fragmentSrc;
	return create(std::move(vertex), std::move(fragment));
}

ShaderProgram* ShaderProgram::create(std::string&& vertexSrc, std::string&& fragmentSrc)
{
	switch (Renderer::getApi())
	{
	case Renderer::Api::OpenGL: return new OpenGLShaderProgram(std::move(vertexSrc), std::move(fragmentSrc));
	}
	return nullptr;
}