#ifndef FLOOR_H
#define FLOOR_H

#include "renderobject.h"

class QOpenGLTexture;

class Floor : public RenderObject
{
public:
  Floor(QOpenGLShaderProgram* s, QOpenGLTexture* t, const QVector3D& pos);

  void draw(const RenderInfo&) override;

protected:
  QVector3D _position;
};

#endif // FLOOR_H
