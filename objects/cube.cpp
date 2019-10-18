#include "cube.h"

#include <QOpenGLTexture>

float cubeVertices[] = {
  // position          // UV          // Normal
  -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,   0.0f,  0.0f, -1.0f,
  0.5f, -0.5f, -0.5f,  1.0f, 0.0f,    0.0f,  0.0f, -1.0f,
  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,    0.0f,  0.0f, -1.0f,
  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,    0.0f,  0.0f, -1.0f,
  -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,    0.0f,  0.0f, -1.0f,
  -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,    0.0f,  0.0f, -1.0f,

  -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,    0.0f,  0.0f, 1.0f,
  0.5f, -0.5f,  0.5f,  1.0f, 0.0f,    0.0f,  0.0f, 1.0f,
  0.5f,  0.5f,  0.5f,  1.0f, 1.0f,    0.0f,  0.0f, 1.0f,
  0.5f,  0.5f,  0.5f,  1.0f, 1.0f,    0.0f,  0.0f, 1.0f,
  -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,    0.0f,  0.0f, 1.0f,
  -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,    0.0f,  0.0f, 1.0f,

  -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,    -1.0f,  0.0f, 0.0f,
  -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,    -1.0f,  0.0f, 0.0f,
  -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,    -1.0f,  0.0f, 0.0f,
  -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,    -1.0f,  0.0f, 0.0f,
  -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,    -1.0f,  0.0f, 0.0f,
  -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,    -1.0f,  0.0f, 0.0f,

  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,    1.0f,  0.0f, 0.0f,
  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,    1.0f,  0.0f, 0.0f,
  0.5f, -0.5f, -0.5f,  0.0f, 1.0f,    1.0f,  0.0f, 0.0f,
  0.5f, -0.5f, -0.5f,  0.0f, 1.0f,    1.0f,  0.0f, 0.0f,
  0.5f, -0.5f,  0.5f,  0.0f, 0.0f,    1.0f,  0.0f, 0.0f,
  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,    1.0f,  0.0f, 0.0f,

  -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,    0.0f,  -1.0f, 0.0f,
  0.5f, -0.5f, -0.5f,  1.0f, 1.0f,    0.0f,  -1.0f, 0.0f,
  0.5f, -0.5f,  0.5f,  1.0f, 0.0f,    0.0f,  -1.0f, 0.0f,
  0.5f, -0.5f,  0.5f,  1.0f, 0.0f,    0.0f,  -1.0f, 0.0f,
  -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,    0.0f,  -1.0f, 0.0f,
  -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,    0.0f,  -1.0f, 0.0f,

  -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,     0.0f,  1.0f, 0.0f,
  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,     0.0f,  1.0f, 0.0f,
  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,     0.0f,  1.0f, 0.0f,
  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,     0.0f,  1.0f, 0.0f,
  -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,     0.0f,  1.0f, 0.0f,
  -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,     0.0f,  1.0f, 0.0f
};

Cube::Cube(QOpenGLShaderProgram* p, QOpenGLTexture* t, const QVector3D& pos)
  : RenderObject(p, t, pos)
{
  // generate and bind VAO
  vertexArrayObject.create();
  QOpenGLVertexArrayObject::Binder binder(&vertexArrayObject);

  // generate, bind and copy VBO
  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 288, cubeVertices, GL_STATIC_DRAW);

  // set the vertex attributes pointers
  //position
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // uv
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // normals
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Cube::draw(const RenderInfo& info)
{
  _program->bind();
  _texture->bind();
  _program->setUniformValue("texture1", 0);

  QMatrix4x4 model;
  model.translate(_position);

  _program->setUniformValue("model", model);
  _program->setUniformValue("view", info.viewMatrix);
  _program->setUniformValue("projection", info.projectionMatrix);
  _program->setUniformValue("lightColor", info.ligthColor);
  _program->setUniformValue("lightPos", info.lightPos);
  _program->setUniformValue("cameraPos", info.cameraPos);

  QOpenGLVertexArrayObject::Binder binder(&vertexArrayObject);
  glDrawArrays(GL_TRIANGLES, 0, 36);

  _program->release();
}
