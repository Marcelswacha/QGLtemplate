#include "lightsource.h"

LightSource::LightSource(QOpenGLShaderProgram *p, QOpenGLTexture *t, const QVector3D &pos, const QVector3D &color)
    : RenderObject(p, t, pos)
    , _color(color)
{
    // generate and bind VAO
    vertexArrayObject.create();
    QOpenGLVertexArrayObject::Binder binder(&vertexArrayObject);


    float vertex[] = { _position.x(), _position.y(), _position.z() };

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float), vertex, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void LightSource::draw(const RenderInfo & info)
{
    QMatrix4x4 model;
    model.translate(_position);

    _program->bind();
    _program->setUniformValue("model", model);
    _program->setUniformValue("view", info.viewMatrix);
    _program->setUniformValue("projection", info.projectionMatrix);
    _program->setUniformValue("lightColor", _color);

    glPointSize(16.0);

    QOpenGLVertexArrayObject::Binder binder(&vertexArrayObject);
    glDrawArrays(GL_POINTS, 0, 1);

    _program->release();
}
