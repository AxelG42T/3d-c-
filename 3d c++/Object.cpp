#include "Object.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

Object::Object(void* data, size_t datasize)
{
	SetData(data, datasize);
}

Object::Object(std::string path)
{
	GetFileContentLine(path.c_str());
	SetData(verticeFloat.data(), verticeFloat.size() * sizeof(float));
}

void Object::draw(int drawMode, int startVertex, int nbVertex)
{
	glBindVertexArray(VAO);
	glDrawArrays(drawMode, startVertex, nbVertex);
	glBindVertexArray(0);
}

void Object::SetVertexAttributes(int locationID, int dimension, int type, size_t sizeByteVertex, void* startOffset)
{
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(locationID, dimension, type, GL_FALSE, sizeByteVertex, startOffset);
	glEnableVertexAttribArray(locationID);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}
void Object::GetFileContentLine(const char* path)
{
	try
	{
		std::fstream objectFile(path);
		std::string line;
		//objectFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		if (!objectFile)
		{
			std::cout << "ERROR : File don't exist " << std::endl;
		}
		else 
		{
			if (!objectFile.eof())
			{
				while (std::getline(objectFile, line))
				{
					//std::cout << line << std::endl;
					std::stringstream stream(line);
					std::string token;
					stream >> token;
					//std::cout << token << std::endl;
					if (token == "v") {
						float x;
						float y;
						float z;
						stream >> x;
						stream >> y;
						stream >> z;
						//std::cout << x << ", " << y << "," << z << std::endl;
						vertices.push_back(glm::vec3(x, y, z));
					}
					if (token == "f")
					{
						//TODO: GESTION DES /
						int x;
						int y;
						int z;
						stream >> x;
						stream >> y;
						stream >> z;
						//std::cout << x << ", " << y << "," << z << std::endl;
						faces.push_back(x);
						faces.push_back(y);
						faces.push_back(z);
					}
					if (token == "vn")
					{
						int x;
						int y;
						int z;
						stream >> x;
						stream >> y;
						stream >> z;
						//std::cout << x << ", " << y << "," << z << std::endl;
						normals.push_back(glm::vec3(x, y, z));
					}
					else if (token == "vt")
					{
						int x;
						int y;
						stream >> x;
						stream >> y;
						//std::cout << x << ", " << y << std::endl;
						textures.push_back(glm::vec2(x, y));
					}
				}
			}
		}

		for (int i = 0; i < faces.size(); ++i) {
			verticebytriangle.push_back(vertices[faces[i] - 1].x);
			verticebytriangle.push_back(vertices[faces[i] - 1].y);
			verticebytriangle.push_back(vertices[faces[i] - 1].z);
		}

		if (normals.size() == 0)
		{
			GenerateNormal();
		}
		else
		{
			for (int i = 0; i < faces.size(); ++i) {
				normalsbytriangle.push_back(normals[faces[i] - 1].x);
				normalsbytriangle.push_back(normals[faces[i] - 1].y);
				normalsbytriangle.push_back(normals[faces[i] - 1].z);
			}
		}

		if (textures.size() > 0)
		{
			for (int i = 0; i < faces.size(); ++i) {
				texturesbytriangle.push_back(textures[faces[i] - 1].x);
				texturesbytriangle.push_back(textures[faces[i] - 1].y);
			}
		}

		for (int i = 0; i < verticebytriangle.size(); i += 3) {
			verticeFloat.push_back(verticebytriangle[i]);
			verticeFloat.push_back(verticebytriangle[i+1]);
			verticeFloat.push_back(verticebytriangle[i+2]);
			verticeFloat.push_back(normalsbytriangle[i]);
			verticeFloat.push_back(normalsbytriangle[i + 1]);
			verticeFloat.push_back(normalsbytriangle[i + 2]);
			if (textures.size() > 0) //TODO: GESTION DES / TEXTURE
			{
				verticeFloat.push_back(vertices[texturesbytriangle[i] - 1].x);
				verticeFloat.push_back(vertices[texturesbytriangle[i] - 1].y);
			}
		}
	}
	catch (std::ifstream::failure& e)
	{
		std::cout << "ERROR : File can't be read " << e.what() << std::endl;
	}
}

void Object::GenerateNormal()
{

	for (int i = 0; i < verticebytriangle.size(); i += 9)
	{
		glm::vec3 p1 = glm::vec3(verticebytriangle[i], verticebytriangle[i+1], verticebytriangle[i + 2]);
		glm::vec3 p2 = glm::vec3(verticebytriangle[i+3], verticebytriangle[i + 4], verticebytriangle[i + 5]);
		glm::vec3 p3 = glm::vec3(verticebytriangle[i+6], verticebytriangle[i + 7], verticebytriangle[i + 8]);

		glm::vec3 u = p2 - p1;
		glm::vec3 v = p3 - p1;

		glm::vec3 normal = glm::cross(u,v);

		normalsbytriangle.push_back(normal.x);
		normalsbytriangle.push_back(normal.y);
		normalsbytriangle.push_back(normal.z);
		normalsbytriangle.push_back(normal.x);
		normalsbytriangle.push_back(normal.y);
		normalsbytriangle.push_back(normal.z);
		normalsbytriangle.push_back(normal.x);
		normalsbytriangle.push_back(normal.y);
		normalsbytriangle.push_back(normal.z);
	}
}

void Object::SetData(void* data, size_t datasize)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, datasize, data, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}
