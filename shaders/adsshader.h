#ifndef ADSSHADER_H
#define ADSSHADER_H

#include "shader.h"

class ADSShader : public Shader
{
public:
    ADSShader();

    void updateLightOptions(const ShaderLighOptions&) override;
    void updateSceneOptions(const ShaderSceneOptions&) override;
    void updateObjectOptions(const ShaderObjectOptions&) override;
    void updateSamplers() override;
};

#endif // ADSSHADER_H
