#pragma once
#include <string>

class ShaderProgram
{
public:
	struct InfoLog
	{
		std::string vertexLog;
		std::string fragmentLog;
	};
protected:
	ShaderProgram(std::string &&vertexSrc, std::string &&fragmentSrc);

public:
	static ShaderProgram* create(const std::string& vertexSrc, const std::string& fragmentSrc);
	static ShaderProgram* create(std::string &&vertexSrc, std::string&& fragmentSrc);
	virtual bool compile(InfoLog* result = nullptr) const = 0;

protected:
	std::string _vertexSrc;
	std::string _fragmentSrc;

};