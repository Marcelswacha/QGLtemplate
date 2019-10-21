#ifndef SHAPE_H
#define SHAPE_H

#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>

class Shape : public QOpenGLFunctions
{
public:
    Shape();
    virtual ~Shape();

    virtual void draw() {}

protected:
    QOpenGLVertexArrayObject _vao;
    GLuint _vbo;
};


#endif // SHAPE_H
