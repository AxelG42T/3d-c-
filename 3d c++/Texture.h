#ifndef TEXTURE_H__
#define TEXTURE_H__

#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include "stb_image.h"

class Texture
{
public:
	Texture(std::string path);
	void activate();
	void deactivate();
private:
	unsigned int texture;
};

#endif