#ifndef RENDEROBJECT_H
#define RENDEROBJECT_H

#include "renderinfo.h"
#include "shader.h"
#include "shape.h"

#include <QOpenGLFunctions>
#include <QOpenGLTexture>

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

  virtual void draw(const RenderInfo&);
  virtual void update();

protected:
  Shader* _program;
  QOpenGLTexture* _texture;
  Shape* _shape;

  QVector3D _position {0.f, 0.f, 0.f};
  QVector3D _velocity {0.f, 0.f, 0.f};
};

#endif // RENDEROBJECT_H
