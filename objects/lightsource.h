#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include "renderobject.h"

class LightSource : public RenderObject
{
public:
  LightSource(Shader* p,
              QOpenGLTexture* t,
              Shape* s,
              const QVector3D& pos,
              const QVector3D& color);

  void update() override {}

  QVector3D pos() const { return _position; }
  QVector3D color() const { return _color; }

protected:
  QVector3D _color;
};

#endif // LIGHTSOURCE_H
