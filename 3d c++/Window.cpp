#include "Window.h"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

Shader* Window::shader = nullptr;

Window::Window() : window(nullptr)
{
	init();
}

Window::~Window()
{

	glfwTerminate();
}

void Window::framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void Window::init()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void Window::window_size_callback(GLFWwindow* window, int width, int height)
{
	if (width == 0 || height == 0)
		return;
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
	shader->SetMatrix4("projection", projection);
}

void Window::SetShader(Shader* shader)
{
	this->shader = shader;
}

void Window::create(int w, int h, std::string title)
{
	window = glfwCreateWindow(w, h, title.c_str(), NULL, NULL);
	if (window == NULL) {
		std::cout << "FAILED TO CREATE WINDOW: " << std::endl;
		glfwTerminate();
		exit(1);
	}
	else {
		glfwMakeContextCurrent(window);
		glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
		glfwSetWindowSizeCallback(window, window_size_callback);
		glfwSwapInterval(1);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "error load glad" << std::endl;
			exit(1);
		}
		glEnable(GL_DEPTH_TEST);
	}
}

bool Window::isOpen()
{
	return !glfwWindowShouldClose(window);
}


void Window::clear(float _r, float _g, float _b)
{
	glClearColor(_r, _g, _b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::pollEvent()
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		close();
	}
	glfwSwapBuffers(window);
	glfwPollEvents();
}

GLFWwindow* Window::getWindow()
{
	return window;
}

void Window::close()
{
	glfwSetWindowShouldClose(window, true);
}
