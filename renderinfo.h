#ifndef RENDERINFO_H
#define RENDERINFO_H

#include <QVector3D>
#include <QMatrix4x4>

struct RenderInfo
{
  QMatrix4x4 viewMatrix;
  QMatrix4x4 projectionMatrix;

  QVector3D ligthColor;
  QVector3D lightPos;

  QVector3D cameraPos;
};


#endif // RENDERINFO_H
