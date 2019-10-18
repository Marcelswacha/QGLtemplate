#ifndef SPHERE_H
#define SPHERE_H

#include "renderobject.h"

class QOpenGLTexture;

class Sphere : public RenderObject
{
public:
  Sphere(QOpenGLShaderProgram* s, QOpenGLTexture* t, const QVector3D& pos);

  void draw(const RenderInfo&) override;
  void update() override;
};

#endif // SPHERE_H
