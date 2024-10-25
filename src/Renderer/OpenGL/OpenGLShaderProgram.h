#pragma once
#include "../ShaderProgram.h"
#include <glad/glad.h>

class OpenGLShaderProgram final : public ShaderProgram
{
public:
	OpenGLShaderProgram(std::string&& vertexSrc, std::string&& fragmentSrc);
	~OpenGLShaderProgram();
	void setVertexShaderSrc(std::string&& vertexSrc) override;
	void setFragmentShaderSrc(std::string&& fragmentSrc) override;
	bool compile(std::string* vertexInfoLog = nullptr, std::string* fragmentInfoLog = nullptr) const override;
	bool link(std::string* infoLog = nullptr) const override;
	void freeSrc() override;
	void bind() const override;
	void unbind() const override;

private:
	GLuint _vertexShaderId{};
	GLuint _fragmentShaderId{};
	GLuint _shaderProgramId{};
};