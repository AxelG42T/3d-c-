#ifndef WINDOW_H__
#define WINDOW_H__

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include "Shader.h"


class Window
{
public:
	Window();
	~Window();

private:
	GLFWwindow* window;
	static Shader* shader;
	
	static void window_size_callback(GLFWwindow* window, int width, int height);
	void init();
	
public:
	static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
	void create(int width, int height, std::string title);
	bool isOpen();
	void clear(float _r, float _g, float _b);
	void pollEvent();
	GLFWwindow* getWindow();
	void close();
	void SetShader(Shader* shader);
};
#endif 