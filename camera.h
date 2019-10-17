#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>
#include <QVector3D>
#include <QMatrix4x4>

class Camera : public QObject
{
    Q_OBJECT
public:
    explicit Camera(QObject *parent = nullptr);

    QMatrix4x4 view();

signals:

public slots:
    void onForwardKeyChanged();
    void onBackwardKeyChanged();
    void onLeftKeyChanged();
    void onRightKeyChanged();
    void mousePress(double x, double y);
    void mouseRelease();
    void mouseMove(double x, double y);
    void update();

private:
    QVector3D pos {0.f, 0.f, 10.f};
    QVector3D front { 0.f, 0.f, -1.f};
    QVector3D right {1.f, 0.f, 0.f};
    QVector3D up {0.f, 1.f, 0.f};

    bool goingForward {false};
    bool goingBackward {false};
    bool goingLeft {false};
    bool goingRight {false};

    bool isMousePressed {false};
    double lastX;
    double lastY;

    double yaw {-90.0};
    double pitch {0.0};

};

#endif // CAMERA_H
