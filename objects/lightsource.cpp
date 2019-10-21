#include "lightsource.h"

LightSource::LightSource(Shader *p,
                         QOpenGLTexture *t,
                         Shape* s,
                         const QVector3D &pos,
                         const QVector3D &color)
    : RenderObject(p, t, s, pos)
    , _color(color)
{
}
