//
// Created by Ale Pisa on 2021-04-02.
//

#include "Shader.h"

Shader::Shader(const char* shaderPath){
    ShaderSourceCode source = parseShader(shaderPath);
    compileShader(source);
}

void Shader::use() {
    glUseProgram(ID);
}

Shader::ShaderSourceCode Shader::parseShader(const char *shaderPath) {
    // Read shaders from file
    std::ifstream stream(shaderPath);

    ShaderType type = ShaderType::NONE;

    std::string line;
    std::stringstream ss[2]; // If I have more shaders in the future I shall increase this

    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if(line.find("vertex") != std::string::npos) {
                type = ShaderType::VERTEX;
            } else if (line.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;
            }
        } else {
            ss[(int)type] << line << "\n";
        }
    }

    return { ss[(int)ShaderType::VERTEX].str(), ss[(int)ShaderType::FRAGMENT].str()};
}

void Shader::compileShader(Shader::ShaderSourceCode &source) {
    // Compile shaders
    // =======================================
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    // Vertex Shader compiling -----------
    const char* vertexSource = source.vertex.c_str();
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexSource, nullptr);
    glCompileShader(vertex);

    // Check for success
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

    // Fragment shader -----------
    const char* fragmentSource = source.fragment.c_str();
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentSource, nullptr);
    glCompileShader(fragment);

    // Check for success
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

    // Create Shader Program ---------------
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);

    // print linking errors if any
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::setUniformFloat(const char *uniformName, float value) {
    glUniform1f(glGetUniformLocation(ID, uniformName), value);
}

void Shader::setUniformInt(const char *uniformName, int value) {
    glUniform1i(glGetUniformLocation(ID, uniformName), value);
}

void Shader::setUNiformBool(const char *uniformName, bool value) {
    glUniform1i(glGetUniformLocation(ID, uniformName), (int)value);
}
