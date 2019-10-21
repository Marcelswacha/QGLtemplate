#ifndef SPHERE_H
#define SPHERE_H

#include "shape.h"

class Sphere : public Shape
{
public:
  Sphere();

  void draw() override;
};

#endif // SPHERE_H
