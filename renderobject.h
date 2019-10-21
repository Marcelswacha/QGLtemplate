#ifndef RENDEROBJECT_H
#define RENDEROBJECT_H

#include "shader.h"
#include "shape.h"

#include <QOpenGLFunctions>

class QOpenGLTexture;

class RenderObject : public QOpenGLFunctions
{
public:
  RenderObject(Shader* p, QOpenGLTexture* t, Shape* s, const QVector3D& pos)
    : _program(p)
    , _texture(t)
    , _shape(s)
    , _position(pos)
  {
      initializeOpenGLFunctions();
  }

  virtual ~RenderObject(){}

  virtual void draw();
  virtual void update();

  void setPosition (float x, float y, float z)
  {
      setVector3D(_position, x, y, z);
  }

  void setRotation (float angle, float x, float y, float z)
  {
      setVector3D(_rotationAxis, x, y, z);
      _rotationAngle = angle;
  }

  void setScale(float x, float y, float z)
  {
      setVector3D(_scale, x, y, z);
  }

protected:
  Shader* _program;
  QOpenGLTexture* _texture;
  Shape* _shape;

  QVector3D _position {0.f, 0.f, 0.f};
  QVector3D _velocity {0.f, 0.f, 0.f};

  QVector3D _rotationAxis {1.f, 0.f, 0.f};
  float     _rotationAngle {0.f};
  QVector3D _scale {1.f, 1.f, 1.f};

  void setVector3D(QVector3D& vec, float x, float y, float z);
};

#endif // RENDEROBJECT_H
