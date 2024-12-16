#ifndef OBJECT_H__
#define OBJECT_H__
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <sstream>

class Object
{
public:
	Object(void* data, size_t datasize);
	Object(std::string path);
	void draw(int drawMode, int startVertex, int nbVertex);
	void SetVertexAttributes(int locationID, int dimension, int type, size_t sizeByteVertex, void* startOffset);
	void GetFileContentLine(const char* path);
	void GenerateNormal();
private:
	unsigned int VBO, VAO;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> textures;
	std::vector<unsigned int> faces;
	std::vector<glm::vec3> all;
	std::vector<float> verticebytriangle;
	std::vector<float> normalsbytriangle;
	std::vector<float> texturesbytriangle;
	std::vector<float> verticeFloat;
	void SetData(void* data, size_t datasize);
	
};
#endif