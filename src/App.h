#pragma once
#include "Window.h"

class App final
{
public:
	App();
	~App() = default;
	int run();

private:
	bool tryCreateMainWindow();
	bool initRendell();

	int _result{ 0 };
	std::unique_ptr<Window> _mainWindow;
};