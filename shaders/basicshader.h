#ifndef BASICSHADER_H
#define BASICSHADER_H

#include "shader.h"

class BasicShader : public Shader
{
public:
    BasicShader();

    void updateLightOptions(const ShaderLightOptions&) override;
    void updateCameraOptions(const ShaderCameraOptions&) override;
    void updateObjectOptions(const ShaderObjectOptions&) override;
};

#endif // BASICSHADER_H
