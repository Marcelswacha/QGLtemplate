#ifndef CUBE_H
#define CUBE_H

#include "renderobject.h"

class QOpenGLTexture;

class Cube : public RenderObject
{
public:
  Cube(QOpenGLShaderProgram* s, QOpenGLTexture* t, const QVector3D& pos);

  void draw(const RenderInfo&) override;
};

#endif // CUBE_H
