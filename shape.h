#ifndef SHAPE_H
#define SHAPE_H

#include "renderinfo.h"

#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>

class Shape : public QOpenGLFunctions
{
public:
    Shape();
    virtual ~Shape();

    virtual void draw() {}

//    virtual void bind();
//    virtual void release();

protected:
    QOpenGLVertexArrayObject _vao;
    GLuint _vbo;
};


#endif // SHAPE_H
