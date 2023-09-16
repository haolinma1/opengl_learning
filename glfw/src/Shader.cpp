#include "GL/glew.h"
#include <iostream>
#include "Shader.h"
#include "fstream"
#include <sstream>
#include "Renderer.h"

// get two shader from a file and store them in struct
ShaderProgramSource Shader::GetShader(const std::string& ShaderPath) {
    std::ifstream readfile(ShaderPath);
    std::string line;

    enum class Shadertype {
        none = -1, vertex = 0, fragment = 1
    };

    std::stringstream ss[2];
    Shadertype type = Shadertype::none;
    while (getline(readfile, line))
    {
        // find the type of shader
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                // find the vertex
                type = Shadertype::vertex;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                // find the fragment
                type = Shadertype::fragment;
            }
        }
        else
        {
            ss[(int)type] << line << "\n";
        }
    }
    // return a struct
    return { ss[0].str(),ss[1].str() };
}

// in the process of create shader
// we need to complie them
unsigned int Shader::ComplieShader(unsigned int type, const std::string& source) {
    
    // glCreateShader creates an empty shader object and returns a non-zero value by which it can be referenced
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    // count means that how many string that you want to pass to this function
    // src means the address of the array of the string
    // NULL means that we don't have the array, hence just one string
    GLDebugger(glShaderSource(id, 1, &src, nullptr));
    GLDebugger(glCompileShader(id));

    // TODO handle error
    int result;
    GLDebugger(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE)
    {
        // get the length of error message first
        int length;
        GLDebugger(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = new char[length];
        GLDebugger(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "Fail to complie: " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
        std::cout << message << std::endl;
        GLDebugger(glDeleteShader(id));
        return 0;
    }


    return id;


}

// for each shader, we need to create it
 unsigned int Shader::CreatShader(const std::string& vertexShader, const std::string& fragmentShader) {
    // create an empty program object and return a non-zero number that we can use it to reference
    unsigned int program = glCreateProgram();

    unsigned int vs = ComplieShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = ComplieShader(GL_FRAGMENT_SHADER, fragmentShader);

    // attach shader to our program
    GLDebugger(glAttachShader(program, vs));
    GLDebugger(glAttachShader(program, fs));

    GLDebugger(glLinkProgram(program));
    GLDebugger(glValidateProgram(program));

    GLDebugger(glDeleteShader(vs));
    GLDebugger(glDeleteShader(fs));

    return program;
}


Shader::Shader(const std::string& FilePath):m_FilePath(FilePath),m_ShaderID(0) {
    // get the string of these two shaders
    ShaderProgramSource shaders= GetShader(FilePath);
    m_ShaderID = CreatShader(shaders.vertex, shaders.fragment);  
}
Shader::~Shader() {
    GLDebugger(glDeleteProgram(m_ShaderID));
}

void Shader::Bind()const {
    GLDebugger(glUseProgram(m_ShaderID));
}
void Shader::UnBind()const {
    GLDebugger(glUseProgram(0));

}

void Shader::SetUniform1f(const std::string& name, float value) {
    GLDebugger(glUniform1f(GetUniformLocation(name), value));
}
void Shader::SetUniform1i(const std::string& name, int value) {
    GLDebugger(glUniform1i(GetUniformLocation(name), value));
}

// 4 stands for how many floating number
// for color, we need 4 floating number
void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
    GLDebugger(glUniform4f(GetUniformLocation(name), v0, v1, v2, v2));
}

// pass in the uniform name
unsigned int Shader::GetUniformLocation(const std::string& name) {

    // use a unorder map to store the location with specific uniform
    if (UniformLocationCache.find(name)!=UniformLocationCache.end())
    {
        return UniformLocationCache[name];
    }
    GLDebugger(int location = glGetUniformLocation(m_ShaderID, name.c_str()));
    if (location ==-1)
    {
        std::cout << "Uniform Name " << name << " doesn't exits" << std::endl;
    }
    UniformLocationCache[name] = location;
    return location;
}
