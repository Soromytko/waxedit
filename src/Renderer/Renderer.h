#pragma once
#include <string>

class Renderer
{
protected:
	Renderer() = default;

public:
	enum class Api
	{
		OpenGL,
	};

	static Api getApi();
	static void setApi(Api api = Api::OpenGL);
	static bool init(std::string *reason = nullptr);

private:
	static bool initApi();
	static bool initFreeType();

	static Api _api;

};