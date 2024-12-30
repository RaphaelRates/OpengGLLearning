#ifndef SHADERPROGRAM_HPP
#define SHADERPROGRAM_HPP

#include <string>
#include <glad/glad.h>
#include <iostream>

class ShaderProgram {
public:
    unsigned int programID;

    ShaderProgram() : programID(0) {}

    void attachShader(const std::string& vertexPath, const std::string& fragmentPath);
    void link();
    void use() const {
        glUseProgram(programID);
    }

private:
    std::string loadShaderSource(const std::string& path);
    unsigned int compileShader(const std::string& source, GLenum shaderType);
};

#endif // SHADERPROGRAM_HPP
