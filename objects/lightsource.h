#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include "renderobject.h"


class QOpenGLTexture;

class LightSource : public RenderObject
{
public:
  LightSource(QOpenGLShaderProgram* p, QOpenGLTexture* t,
              const QVector3D& pos,
              const QVector3D& color);

  void draw(const RenderInfo&) override;

  QVector3D pos() const { return _position; }
  QVector3D color() const { return _color; }

protected:
  QVector3D _color;
};

#endif // LIGHTSOURCE_H
