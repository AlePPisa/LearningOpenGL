//
// Created by Ale Pisa on 2021-04-02.
//

#ifndef LEARNOPENGL_SHADER_H
#define LEARNOPENGL_SHADER_H

#include <fstream>
#include <string>
#include <sstream>
#include <glad/glad.h>
#include <iostream>

class Shader {
public:
    unsigned int ID; // Shader Program ID

    explicit Shader(const char* shaderPath);

    virtual ~Shader();

    void use() const;

    void setUniformFloat(const char* uniformName, float value) const;

    void setUniformInt(const char* uniformName, int value) const;

    void setUNiformBool(const char* uniformName, bool value) const;

private:
    struct ShaderSourceCode {
        std::string vertex;
        std::string fragment;
    };

    enum ShaderType {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };

    static ShaderSourceCode parseShader(const char* shaderPath);

    void compileShader(ShaderSourceCode &source);
};

#endif //LEARNOPENGL_SHADER_H
