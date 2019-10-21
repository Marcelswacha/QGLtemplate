#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QKeyEvent>

class QTimer;
class QOpenGLTexture;

class Camera;
class LightSource;
class RenderObject;
class Shader;
class ShaderCameraOptions;
class ShaderLightOptions;
class Shape;

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    QSize minimumSizeHint() const override;

public slots:
    void cleanup();
    void setLightColor(float r, float g, float b);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    void wheelEvent(QWheelEvent* event) override;

    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    QOpenGLTexture* _cubeTexture;
    QOpenGLTexture* _floorTexture;
    QOpenGLTexture* _footballTexture;

    Shader* _objectProgram;
    Shader* _lightProgram;
    ShaderCameraOptions* _cameraOptions;
    ShaderLightOptions* _lightOptions;

    Shape* _sphereShape;
    Shape* _cubeShape;
    Shape* _floorShape;

    Camera* _camera;
    LightSource* _lightSource;
    RenderObject* _floor;
    QVector<RenderObject*> _objects;

    QTimer* _timer;

    void generateNewObject();
    void updateCamera();
    void updateLights();
    void updateCameraOptions(Shader* program);
    void updateLightOptions(Shader* program);
};

#endif
