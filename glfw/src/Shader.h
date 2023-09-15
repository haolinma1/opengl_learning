#pragma once
#include <string>
#include <unordered_map>


struct ShaderProgramSource {
	std::string vertex;
	std::string fragment;
};
class Shader
{
private:
	// for debug
	std::string m_FilePath;
	unsigned int m_ShaderID;
	std::unordered_map<std::string, int> UniformLocationCache;
public:
	Shader(const std::string& FilePath);
	~Shader();

	void Bind()const;
	void UnBind()const;

	// 4 stands for how many floating number
	// for color, we need 4 floating number
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);

private:
	unsigned int GetUniformLocation(const std::string& name);
	ShaderProgramSource GetShader(const std::string& ShaderPath);
	unsigned int CreatShader(const std::string& vertexShader, const std::string& fragmentShader);
	unsigned int ComplieShader(unsigned int type, const std::string& source);

};

