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
    return QSize(600, 800);
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

void GLWidget::setLightColor(float r, float g, float b)
{
    QVector3D color(r, g, b);
    _lightSource->setColor(color);
    updateLights();
}

void GLWidget::initializeGL()
{
    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &GLWidget::cleanup);

    initializeOpenGLFunctions();
    glClearColor(0.f, 0.f, 0.f, 1.0f);
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
    _cameraOptions = new ShaderCameraOptions{_camera->pos(), _camera->projection(devicePixelRatio()), _camera->view() };

    // lights
    _lightSource = new LightSource(_lightProgram, nullptr, _sphereShape, QVector3D(3,15,3), QVector3D(1, 1, 1));
    _lightOptions = new ShaderLightOptions{_lightSource->pos(), _lightSource->color()};

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

    glClearColor(0.f, 0.f, 0.f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    updateCamera();
    updateLights();

    _lightSource->draw();
    _floor->draw();
    for (int i = 0; i < _objects.size(); ++i) {
       _objects[i]->update();
       _objects[i]->draw();
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

void GLWidget::updateCamera()
{
    _camera->update();
    _cameraOptions->cameraPos = _camera->pos();
    _cameraOptions->projection = _camera->projection(devicePixelRatio());
    _cameraOptions->view = _camera->view();

    updateCameraOptions(_objectProgram);
    updateCameraOptions(_lightProgram);
}

void GLWidget::updateLights()
{
    _lightOptions->lightPos = _lightSource->pos();
    _lightOptions->lightColor = _lightSource->color();

    updateLightOptions(_objectProgram);
    updateLightOptions(_lightProgram);
}

void GLWidget::updateCameraOptions(Shader *program)
{
    program->bind();
    program->updateCameraOptions(*_cameraOptions);
    program->release();
}

void GLWidget::updateLightOptions(Shader *program)
{
    program->bind();
    program->updateLightOptions(*_lightOptions);
    program->release();
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
