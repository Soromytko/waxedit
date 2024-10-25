#include "OpenGLShaderProgram.h"

static GLuint createShader(const std::string& src, GLuint type)
{
	GLuint result = glCreateShader(type);
	const GLchar* srcPtr = static_cast<const GLchar*>(src.c_str());
	glShaderSource(result, 1, &srcPtr, NULL);
	return result;
}

OpenGLShaderProgram::OpenGLShaderProgram(std::string&& vertexSrc, std::string&& fragmentSrc) :
	ShaderProgram(std::move(vertexSrc), std::move(fragmentSrc))
{
	_vertexShaderId = createShader(_vertexSrc, GL_VERTEX_SHADER);
	_fragmentShaderId = createShader(_fragmentSrc, GL_FRAGMENT_SHADER);
}

OpenGLShaderProgram::~OpenGLShaderProgram()
{
	glDeleteShader(_vertexShaderId);
	glDeleteShader(_fragmentShaderId);
}

static bool IsShaderCompiledSuccessfully(GLuint shaderId, std::string& log, bool extractLog = true)
{
	int success;
	glCompileShader(shaderId);
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
	if (!success && extractLog)
	{
		static const size_t maxLogLength = 512;
		char infoLog[maxLogLength];
		glGetShaderInfoLog(shaderId, maxLogLength, NULL, infoLog);
		log = infoLog;
	}
	return success;
}

bool OpenGLShaderProgram::compile(InfoLog* result) const
{
	std::string vertexLog, fragmentLog;
	const bool extractLog = result != nullptr;
	const bool vertexResult = IsShaderCompiledSuccessfully(_vertexShaderId, vertexLog, extractLog);
	const bool fragmentResult = IsShaderCompiledSuccessfully(_fragmentShaderId, fragmentLog, extractLog);

	if (result)
	{
		result->vertexLog = vertexLog;
		result->fragmentLog = fragmentLog;
	}

	return vertexResult && fragmentResult;
}

