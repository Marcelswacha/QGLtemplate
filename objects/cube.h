#ifndef CUBE_H
#define CUBE_H

#include "shape.h"

class QOpenGLTexture;

class Cube : public Shape
{
public:
  Cube();

  void draw() override;
};

#endif // CUBE_H
