#ifndef SHADER_H__
#define SHADER_H__

#include <glm/glm.hpp>
#include <string>

class Shader
{
public:
	unsigned int ID;
	unsigned int vertexShader;
	unsigned int fragmentShader;
	Shader(const char* vertexPath, const char* fragmentPath);

	void Process(std::string vertexCode, std::string fragmentCode);

	void Use() const;
	void Shutdown() const;
	void SetVector3(const std::string& name, float x, float y, float z) const;
	void SetVector3(const std::string& name, const glm::vec3& value) const;
	void SetFloat(const std::string& name, float value) const;
	void SetMatrix4(const std::string &name, const glm::mat4 &value) const;
	void SetInt(const std::string& name, int value) const;

private:
	std::string GetFileContent(const char* path);
};

#endif