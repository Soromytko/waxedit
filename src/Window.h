#pragma once
#include <GLFW/glfw3.h>

class Window
{
public:
	Window(int width, int height, const char *title);
	~Window();
	bool isOpen();
	void makeContextCurrent();
	void swapBuffers();
	void processEvents();
	static bool isInitialized();
	static int getWindowCount();

private:
	bool init();
	
	GLFWwindow* _glfwWindow;
	static bool _glfwInitialized;
	static int _windowCount;
};