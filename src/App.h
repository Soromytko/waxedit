#pragma once
#include "Window.h"
#include "Rendering/TextRenderer.h"

class App final
{
public:
	App();
	~App() = default;
	int run();

private:
	bool tryCreateMainWindow();
	bool initRendell();
	bool initTextRenderer();
	void setupTextRenderer();

	int _result{ 0 };
	std::unique_ptr<Window> _mainWindow;
	std::unique_ptr<TextRenderer> _textRenderer;
};