#ifndef RENDEROBJECT_H
#define RENDEROBJECT_H

#include <QVector3D>
#include <QMatrix4x4>

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLTexture>

struct RenderInfo
{
  QMatrix4x4 viewMatrix;
  QMatrix4x4 projectionMatrix;

  QVector3D ligthColor;
  QVector3D lightPos;

  QVector3D cameraPos;
};

struct RenderObject : protected QOpenGLFunctions
{
  QOpenGLShaderProgram* _program;
  QOpenGLTexture* _texture;
  QOpenGLVertexArrayObject vertexArrayObject;

  QVector3D _position {0.f, 0.f, 0.f};
  QVector3D _velocity {0.f, 0.f, 0.f};

  RenderObject(QOpenGLShaderProgram* p, QOpenGLTexture* t, const QVector3D& pos)
    : _program(p)
    , _texture(t)
    , _position(pos)
  {
      initializeOpenGLFunctions();
  }

  virtual ~RenderObject(){}

  virtual void draw(const RenderInfo&) {}
  virtual void update() {}
};

#endif // RENDEROBJECT_H
