#include "basicshader.h"

static const char* basicShaderVSPath = ":/shaders/lightvs.glsl";
static const char* basicShaderFSPath = ":/shaders/lightfs.glsl";

BasicShader::BasicShader()
    : Shader(basicShaderVSPath,
             basicShaderFSPath)
{
}

void BasicShader::updateLightOptions(const ShaderLighOptions& lightOptions)
{
    _program->setUniformValue("lightColor", lightOptions.lightColor);
}

void BasicShader::updateSceneOptions(const ShaderSceneOptions& sceneOptions)
{
    _program->setUniformValue("view", sceneOptions.view);
    _program->setUniformValue("projection", sceneOptions.projection);
}

void BasicShader::updateObjectOptions(const ShaderObjectOptions& objectOptions)
{
    _program->setUniformValue("model", objectOptions.model);
}
