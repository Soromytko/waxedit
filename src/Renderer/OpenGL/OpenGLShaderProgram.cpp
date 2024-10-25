#include "OpenGLShaderProgram.h"
#define LOG_INFO_BUFFER_SIZE 512

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
	_shaderProgramId = glCreateProgram();
}

OpenGLShaderProgram::~OpenGLShaderProgram()
{
	glDeleteShader(_vertexShaderId);
	glDeleteShader(_fragmentShaderId);
	glDeleteProgram(_shaderProgramId);
}

void OpenGLShaderProgram::setVertexShaderSrc(std::string&& vertexSrc)
{
	ShaderProgram::setVertexShaderSrc(std::move(vertexSrc));
	glDeleteShader(_vertexShaderId);
	_vertexShaderId = createShader(_vertexSrc, GL_VERTEX_SHADER);
}

void OpenGLShaderProgram::setFragmentShaderSrc(std::string&& fragmentSrc)
{
	ShaderProgram::setFragmentShaderSrc(std::move(fragmentSrc));
	glDeleteShader(_fragmentShaderId);
	_fragmentShaderId = createShader(_fragmentSrc, GL_FRAGMENT_SHADER);
}

static bool IsShaderCompiledSuccessfully(GLuint shaderId, std::string* infoLog)
{
	int success;
	glCompileShader(shaderId);
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
	if (!success && infoLog)
	{
		char infoLogBuffer[LOG_INFO_BUFFER_SIZE];
		glGetShaderInfoLog(shaderId, LOG_INFO_BUFFER_SIZE, NULL, infoLogBuffer);
		*infoLog = infoLogBuffer;
	}
	return success;
}

bool OpenGLShaderProgram::compile(std::string* vertexInfoLog, std::string* fragmentInfoLog) const
{
	const bool vertexResult = IsShaderCompiledSuccessfully(_vertexShaderId, vertexInfoLog);
	const bool fragmentResult = IsShaderCompiledSuccessfully(_fragmentShaderId, fragmentInfoLog);
	return vertexResult && fragmentResult;
}

bool OpenGLShaderProgram::link(std::string* infoLog) const
{
	glAttachShader(_shaderProgramId, _vertexShaderId);
	glAttachShader(_shaderProgramId, _fragmentShaderId);
	glLinkProgram(_shaderProgramId);

	int success;
	glGetProgramiv(_shaderProgramId, GL_LINK_STATUS, &success);
	if (!success && infoLog)
	{
		char infoLogBuffer[LOG_INFO_BUFFER_SIZE];
		glGetProgramInfoLog(_shaderProgramId, LOG_INFO_BUFFER_SIZE, NULL, infoLogBuffer);
		*infoLog = infoLogBuffer;
	}
	return success;
}

void OpenGLShaderProgram::freeSrc()
{
	glDeleteShader(_vertexShaderId);
	glDeleteShader(_fragmentShaderId);
	_vertexSrc.clear();
	_fragmentSrc.clear();
}

void OpenGLShaderProgram::bind() const
{
	glUseProgram(_shaderProgramId);
}

void OpenGLShaderProgram::unbind() const
{
	glUseProgram(0);
}

