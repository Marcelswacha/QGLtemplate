#ifndef ADSSHADER_H
#define ADSSHADER_H

#include "shader.h"

class ADSShader : public Shader
{
public:
    ADSShader();

    void updateLightOptions(const ShaderLightOptions&) override;
    void updateCameraOptions(const ShaderCameraOptions&) override;
    void updateObjectOptions(const ShaderObjectOptions&) override;
    void updateSamplers() override;
};

#endif // ADSSHADER_H
