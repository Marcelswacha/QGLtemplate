#include "adsshader.h"

static const char* basicShaderVSPath = ":/shaders/vertexshader.glsl";
static const char* basicShaderFSPath = ":/shaders/fragmentshader.glsl";

ADSShader::ADSShader()
    : Shader(basicShaderVSPath,
             basicShaderFSPath)
{
}

void ADSShader::updateLightOptions(const ShaderLighOptions& lightOptions)
{
    _program->setUniformValue("lightColor", lightOptions.lightColor);
    _program->setUniformValue("lightPos", lightOptions.lightPos);
}

void ADSShader::updateSceneOptions(const ShaderSceneOptions& sceneOptions)
{
    _program->setUniformValue("cameraPos", sceneOptions.cameraPos);
    _program->setUniformValue("view", sceneOptions.view);
    _program->setUniformValue("projection", sceneOptions.projection);
}

void ADSShader::updateObjectOptions(const ShaderObjectOptions& objectOptions)
{
    _program->setUniformValue("model", objectOptions.model);
}

void ADSShader::updateSamplers()
{
    _program->setUniformValue("texture1", 0);
}
