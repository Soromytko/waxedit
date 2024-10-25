#pragma once
#include <string>

class ShaderProgram
{
protected:
	ShaderProgram(std::string&& vertexSrc, std::string&& fragmentSrc);

public:
	static ShaderProgram* create(const std::string& vertexSrc, const std::string& fragmentSrc);
	static ShaderProgram* create(std::string&& vertexSrc, std::string&& fragmentSrc);
	void setVertexShaderSrc(const std::string& vertexSrc);
	void setFragmentShaderSrc(const std::string& fragmentSrc);
	virtual void setVertexShaderSrc(std::string&& vertexSrc);
	virtual void setFragmentShaderSrc(std::string&& fragmentSrc);
	virtual bool compile(std::string* vertexInfoLog = nullptr, std::string* fragmentInfoLog = nullptr) const = 0;
	virtual bool link(std::string* infoLog = nullptr) const = 0;
	virtual void freeSrc() = 0;
	virtual void bind() const = 0;
	virtual void unbind() const = 0;

protected:
	std::string _vertexSrc;
	std::string _fragmentSrc;

};