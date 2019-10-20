#ifndef SHADER_H
#define SHADER_H

#include <memory>

#include <QVector3D>
#include <QMatrix4x4>

#include <QOpenGLShaderProgram>

struct ShaderLighOptions
{
    QVector3D lightPos;
    QVector3D lightColor;
};

struct ShaderSceneOptions
{
    QVector3D  cameraPos;
    QMatrix4x4 projection;
    QMatrix4x4 view;
};

struct ShaderObjectOptions
{
    QMatrix4x4 model;
};

class Shader
{
public:
    Shader(const char* vsPath,
           const char* fsPath);

    virtual ~Shader() {}

    void bind();
    void release();

    virtual void updateLightOptions(const ShaderLighOptions&) {}
    virtual void updateSceneOptions(const ShaderSceneOptions&) {}
    virtual void updateObjectOptions(const ShaderObjectOptions&) {}
    virtual void updateSamplers() {}

protected:
    std::unique_ptr<QOpenGLShaderProgram> _program;
};

#endif // SHADER_H
