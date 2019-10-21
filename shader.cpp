#include "shader.h"

#include <stdexcept>

Shader::Shader(const char* vsPath,
               const char* fsPath)
{
    _program.reset(new QOpenGLShaderProgram);
    if (!_program->addShaderFromSourceFile(QOpenGLShader::Vertex, vsPath))
        throw std::runtime_error("Cannot load vertex shader from file");

    if (!_program->addShaderFromSourceFile(QOpenGLShader::Fragment, fsPath))
        throw std::runtime_error("Cannot load fragment shader from file");

    if (!_program->link())
        throw std::runtime_error("Cannot load shader program");

    if (!_program->bind())
        throw std::runtime_error("Cannot bind shader program");

    _program->release();
}

void Shader::bind()
{
    _program->bind();
}

void Shader::release()
{
    _program->release();
}
