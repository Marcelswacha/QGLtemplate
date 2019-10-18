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

#include "objects/cube.h"
#include "objects/floor.h"
#include "objects/lightsource.h"
#include "objects/sphere.h"

#include "camera.h"

#include <QCoreApplication>
#include <QMouseEvent>
#include <QTimer>

#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

#include <cmath>
#include <random>

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
    , _objectProgram(nullptr)
    , _lightProgram(nullptr)
{
    _timer = new QTimer(this);
    connect(_timer, SIGNAL(timeout()), this, SLOT(update()));
    _timer->start(10);

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
    return QSize(1440, 900);
}

void GLWidget::cleanup()
{
    makeCurrent();

    delete _lightSource;

    delete _objectProgram;
    _objectProgram = nullptr;

    delete _lightProgram;
    _lightProgram = nullptr;

    doneCurrent();
}

void GLWidget::initializeGL()
{
    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &GLWidget::cleanup);

    initializeOpenGLFunctions();
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    // Shaders
    _objectProgram = createProgram(":/shaders/vertexshader.glsl", ":/shaders/fragmentshader.glsl");
    if (_objectProgram == nullptr) {
        close();
    }

    _lightProgram = createProgram(":/shaders/lightvs.glsl", ":/shaders/lightfs.glsl");
    if (_lightProgram == nullptr) {
        close();
    }


    // Textures
    _cubeTexture = new QOpenGLTexture(QImage(QString(":/textures/chest.jpg")));
    _floorTexture = new QOpenGLTexture(QImage(QString(":/textures/stone.jpg")));
    _footballTexture = new QOpenGLTexture(QImage(QString(":/textures/football1.jpg")));

    // Scene
    // camera
    _camera = new Camera(this);

    // lights
    _lightSource = new LightSource(_lightProgram, nullptr, QVector3D(3,3,3), QVector3D(1, 1, 1));

    static const QVector3D positions[] = {
      QVector3D( 0.0f,  1.5f,  -10.0f),
      QVector3D( 0.0f,  51.0f, .0f),
      QVector3D(-1.5f, 31.2f, -2.5f),
      QVector3D(-3.8f, 31.0f, -12.3f),
      QVector3D( 2.4f, 11.4f, -3.5f),
      QVector3D(-1.7f,  41.0f, -7.5f),
      QVector3D( 1.3f, 31.0f, -2.5f),
      QVector3D( -1.5f,  31.0f, -2.5f),
      QVector3D( 1.5f,  11.2f, -1.5f),
      QVector3D(-1.3f,  21.0f, -1.5f)
    };

    for (int i = 0; i < 10; ++i) {
        _objects.push_back(new Sphere(_objectProgram, _footballTexture, positions[i]));
    }
}

void GLWidget::paintGL()
{
    static int frame = 0;
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _camera->update();

    QMatrix4x4 proj;
    proj.perspective(_camera->fov(), width() / height(), 0.1f, 100.f);

    RenderInfo info;
    info.cameraPos = _camera->pos();
    info.lightPos = _lightSource->pos();
    info.ligthColor = _lightSource->color();
    info.projectionMatrix = proj;
    info.viewMatrix = _camera->view();

    Floor f(_objectProgram, _floorTexture, QVector3D(0,-0.5,0));
    f.draw(info);
    for (int i = 0; i < _objects.size(); ++i) {
       _objects[i]->update();
       _objects[i]->draw(info);
    }

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

    if (event->buttons() & Qt::MiddleButton) {
      _camera->mousePress(x, y);
    }
    else if (event->buttons() & Qt::LeftButton) {
        generateNewObject();
    }
}

void GLWidget::generateNewObject()
{
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> distx(-10,10);
    std::uniform_int_distribution<std::mt19937::result_type> disty(3,20);
    std::uniform_int_distribution<std::mt19937::result_type> distz(-10, 10);

    int x = distx(rng);
    int y = disty(rng);
    int z = distz(rng);

    qDebug() << x << y << z;

    _objects.push_back(new Sphere(_objectProgram, _footballTexture, QVector3D(x, y, z)));
}

void GLWidget::mouseReleaseEvent(QMouseEvent */*event*/)
{
    _camera->mouseRelease();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    double x = event->pos().x() / (double)width();
    double y = event->pos().y() / (double)height();

    _camera->mouseMove(x, y);
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
    _camera->onScroll(event->delta());
}

void GLWidget::keyPressEvent(QKeyEvent *event)
{
    auto key = event->key();
    if (key == Qt::Key_W) {
        _camera->onForwardKeyChanged();
        return;
    }
    if (key == Qt::Key_S) {
        _camera->onBackwardKeyChanged();
        return;
    }
    if (key == Qt::Key_A) {
        _camera->onLeftKeyChanged();
        return;
    }
    if (key == Qt::Key_D) {
        _camera->onRightKeyChanged();
        return;
    }

    QWidget::keyPressEvent(event);
}

void GLWidget::keyReleaseEvent(QKeyEvent *event)
{
    auto key = event->key();
    if (key == Qt::Key_W) {
        _camera->onForwardKeyChanged();
        return;
    }
    if (key == Qt::Key_S) {
        _camera->onBackwardKeyChanged();
        return;
    }
    if (key == Qt::Key_A) {
        _camera->onLeftKeyChanged();
        return;
    }
    if (key == Qt::Key_D) {
        _camera->onRightKeyChanged();
        return;
    }

    QWidget::keyReleaseEvent(event);
}

QOpenGLShaderProgram *GLWidget::createProgram(const char *vsPath, const char *fsPath)
{
    QOpenGLShaderProgram* program = new QOpenGLShaderProgram;
    if (!program->addShaderFromSourceFile(QOpenGLShader::Vertex, vsPath))
        return nullptr;

    if (!program->addShaderFromSourceFile(QOpenGLShader::Fragment, fsPath))
        return nullptr;

    if (!program->link())
        return nullptr;

    if (!program->bind())
        return nullptr;

    program->release();

    return program;
}
