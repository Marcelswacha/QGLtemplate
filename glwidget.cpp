/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "glwidget.h"

#include <QMouseEvent>
#include <QCoreApplication>
#include <QTimer>

#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

#include <math.h>

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
    ,  m_program(nullptr)
{
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(update()));
    m_timer->start(10);

    setFocusPolicy(Qt::ClickFocus);
}

GLWidget::~GLWidget()
{
    cleanup();
}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
    return QSize(800, 600);
}

void GLWidget::cleanup()
{
    makeCurrent();
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ebo);
    delete m_program;
    m_program = 0;
    doneCurrent();
}

void GLWidget::initializeGL()
{
    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &GLWidget::cleanup);

    initializeOpenGLFunctions();
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    m_camera = new Camera(this);

    m_program = new QOpenGLShaderProgram;
    if (!m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertexshader.glsl"))
        close();

    if (!m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragmentshader.glsl"))
        close();

    if (!m_program->link())
        close();

    if (!m_program->bind())
        close();

    m_colorLoc = m_program->uniformLocation("ourColor");

    m_vao.create();
    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);

    float vertices[] = {
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
        };

//    GLuint indices[] = {
//       0, 1, 3, // first triangle
//       1, 2, 3  // second triangle
//   };

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

//    glGenBuffers(1, &m_ebo);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    m_texture = new QOpenGLTexture(QImage(QString(":/textures/container.jpg")).mirrored(true, true));
    // Store the vertex attribute bindings for the program.
    setupVertexAttribs();

    m_program->release();
}

void GLWidget::setupVertexAttribs()
{
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    f->glEnableVertexAttribArray(0);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);

    f->glEnableVertexAttribArray(1);
    f->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GLWidget::paintGL()
{
    static int frame = 0;
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    static const QVector3D cubePositions[] = {
      QVector3D( 0.0f,  0.0f,  0.0f),
      QVector3D( 0.0f,  4.0f, .0f),
//      QVector3D(-1.5f, 2.2f, -2.5f),
//      QVector3D(-3.8f, 2.0f, -12.3f),
//      QVector3D( 2.4f, 0.4f, -3.5f),
//      QVector3D(-1.7f,  3.0f, -7.5f),
//      QVector3D( 1.3f, 2.0f, -2.5f),
//      QVector3D( -1.5f,  2.0f, -2.5f),
//      QVector3D( 1.5f,  0.2f, -1.5f),
//      QVector3D(-1.3f,  1.0f, -1.5f)
    };


//    QVector3D eye(0, 25, 9);
//    QVector3D target(0, 0, 0);
//    QVector3D cameraDir = eye - target; cameraDir.normalize();
//    QVector3D up(0, 1, 0);
//    QVector3D cameraRight = QVector3D::crossProduct(up, cameraDir); cameraRight.normalize();
//    QVector3D cameraUp = QVector3D::crossProduct(cameraDir, cameraRight); cameraUp.normalize();

    m_camera->update();

    QMatrix4x4 view = m_camera->view();

    QMatrix4x4 proj;
    proj.perspective(45.f, width() / height(), 0.1f, 100.f);


    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);
    m_program->bind();


    m_program->setUniformValue("view", view);
    m_program->setUniformValue("projection", proj);
    m_program->setUniformValue("texture1", 0);
    m_texture->bind();

    for (int i = 0; i < 2; ++i) {
        QMatrix4x4 model;
        model.translate(cubePositions[i]);
        float angle = (i + 1) * frame;
        model.rotate(angle, 1.0, 0.3, 0.5);
        m_program->setUniformValue("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

    }



    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    m_program->release();
    ++frame;
}

void GLWidget::resizeGL(int w, int h)
{
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, w * retinaScale, h * retinaScale);
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    double x = event->pos().x() /(double) width();
    double y = event->pos().y() / (double)height();

    m_camera->mousePress(x, y);
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    m_camera->mouseRelease();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    double x = event->pos().x() / (double)width();
    double y = event->pos().y() / (double)height();

    m_camera->mouseMove(x, y);
}

void GLWidget::keyPressEvent(QKeyEvent *event)
{
    auto key = event->key();
    if (key == Qt::Key_W) {
        m_camera->onForwardKeyChanged();
        return;
    }
    if (key == Qt::Key_S) {
        m_camera->onBackwardKeyChanged();
        return;
    }
    if (key == Qt::Key_A) {
        m_camera->onLeftKeyChanged();
        return;
    }
    if (key == Qt::Key_D) {
        m_camera->onRightKeyChanged();
        return;
    }

    QWidget::keyPressEvent(event);
}

void GLWidget::keyReleaseEvent(QKeyEvent *event)
{
    auto key = event->key();
    if (key == Qt::Key_W) {
        m_camera->onForwardKeyChanged();
        return;
    }
    if (key == Qt::Key_S) {
        m_camera->onBackwardKeyChanged();
        return;
    }
    if (key == Qt::Key_A) {
        m_camera->onLeftKeyChanged();
        return;
    }
    if (key == Qt::Key_D) {
        m_camera->onRightKeyChanged();
        return;
    }

    QWidget::keyReleaseEvent(event);
}
