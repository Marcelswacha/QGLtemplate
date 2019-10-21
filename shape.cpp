#include "shape.h"

Shape::Shape()
{
    initializeOpenGLFunctions();

    _vao.create();
    QOpenGLVertexArrayObject::Binder binder(&_vao);

    glGenBuffers(1, &_vbo);
}

Shape::~Shape()
{
    glDeleteBuffers(1, &_vbo);
    _vao.destroy();
}
