#include "glwidget.h"

#include "objects/cube.h"
#include "objects/floor.h"
#include "objects/lightsource.h"
#include "objects/sphere.h"

#include "camera.h"

#include "shaders/adsshader.h"
#include "shaders/basicshader.h"

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
    try {
    _objectProgram = new ADSShader();
    _lightProgram = new BasicShader();
    }
    catch (const std::exception& e) {
        qDebug() << e.what();
        close();
    }

    // Textures
    _cubeTexture = new QOpenGLTexture(QImage(QString(":/textures/chest.jpg")));
    _floorTexture = new QOpenGLTexture(QImage(QString(":/textures/container.jpg")));
    _footballTexture = new QOpenGLTexture(QImage(QString(":/textures/football1.jpg")));

    // Shapes
    _floorShape = new Floor();
    _sphereShape = new Sphere();
    _cubeShape = new Cube();


    // Scene
    // camera
    _camera = new Camera(this);

    // lights
    _lightSource = new LightSource(_lightProgram, nullptr, _cubeShape, QVector3D(3,3,3), QVector3D(1, 1, 1));

    // floor
    _floor = new RenderObject(_objectProgram, _floorTexture, _floorShape, QVector3D(0, -0, 0));
    _floor->setScale(30, 1, 30);
    _floor->setRotation(-90, 1, 0, 0);

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
        _objects.push_back(new RenderObject(_objectProgram, _footballTexture, _sphereShape, positions[i]));
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

    _lightSource->draw(info);
    _floor->draw(info);
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

    _objects.push_back(new RenderObject(_objectProgram, _footballTexture, _sphereShape, QVector3D(x, y, z)));
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
