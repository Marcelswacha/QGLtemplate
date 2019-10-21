#ifndef FLOOR_H
#define FLOOR_H

#include "shape.h"

class Floor : public Shape
{
public:
  Floor();

  void draw() override;
};

#endif // FLOOR_H
