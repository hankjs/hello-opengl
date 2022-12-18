#include "Shader.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

Shader::Shader(const std::string& filepath)
	:m_FilePath(filepath), m_RendererID(0)
{
    ShaderProgramSource source = ParseShader(filepath);
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3)); 
}

ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);
    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) { 
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;
            }
        }
        else {
            ss[(int)type] << line << '\n';
        }
    }
    return { ss[0].str(),ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id;
    /* ?????????? */
    GLCall(id = glCreateShader(type));
    const char* src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE) {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* msg = (char*)_malloca(length * sizeof(char));
        GLCall(glGetShaderInfoLog(id, length, &length, msg)); 
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
        std::cout << msg << std::endl;
        GLCall(glDeleteShader(id));
        return 0;
    }

    return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program;
    GLCall(program = glCreateProgram());
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}

int Shader::GetUniformLocation(const std::string& name)
{
    if (m_UniformlocationCache.find(name) != m_UniformlocationCache.end()) {
        return m_UniformlocationCache[name];
    }
    int location;
    GLCall(location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1) {
        std::cout << "Warning: uniform '" << name << "' doesn't exist" << std::endl;
    }

    m_UniformlocationCache[name] = location;
    return location;
}
