#include "cube.h"

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

Cube::Cube()
  : Shape()
{
  QOpenGLVertexArrayObject::Binder binder(&_vao);

  // copy VBO
  glBindBuffer(GL_ARRAY_BUFFER, _vbo);
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

void Cube::draw()
{
  QOpenGLVertexArrayObject::Binder binder(&_vao);
  glDrawArrays(GL_TRIANGLES, 0, 36);
}
