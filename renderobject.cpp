#include "renderobject.h"

void RenderObject::draw(const RenderInfo& info)
{
    if (_texture != nullptr) {
        _texture->bind();
    }

    QMatrix4x4 model;
    model.translate(_position);

    _program->bind();
    _program->updateSamplers();

    ShaderObjectOptions obj{model};
    _program->updateObjectOptions(obj);

    ShaderSceneOptions scene{info.cameraPos, info.projectionMatrix, info.viewMatrix};
    _program->updateSceneOptions(scene);

    ShaderLighOptions light{info.lightPos, info.ligthColor};
    _program->updateLightOptions(light);

    _shape->draw();

    _program->release();

    if (_texture != nullptr) {
        _texture->release();
    }
}

void RenderObject::update()
{
    static const float delta = 0.05f;
    static const QVector3D g_acc(0, -9.81, 0);
    static const float v_eps = 0.005f;

    if (_position.y() <= 1.0 && _velocity.y() == 0.f) {
        return;
    }

    _position += _velocity * delta + g_acc * delta * delta / 2;
    _velocity += g_acc * delta;

    if (_position.y() <= 1.05 && _velocity.y() < 0.f) {
        _velocity *= -1.f / 1.21;
        _position.setY(1.0);
        if (_velocity.y() <= v_eps) {
            _velocity.setY(0.f);
        }
    }
}
