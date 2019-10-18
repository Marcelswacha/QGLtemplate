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

  RenderObject(QOpenGLShaderProgram* p, QOpenGLTexture* t)
    : _program(p)
    , _texture(t)
  {
      initializeOpenGLFunctions();
  }

  virtual ~RenderObject(){}

  virtual void draw(const RenderInfo&) {}
  virtual void update() {}
};

#endif // RENDEROBJECT_H
