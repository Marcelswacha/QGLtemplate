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
    double fov() const { return _fov; }
    QVector3D pos() const { return _pos; }

public slots:
    void onForwardKeyChanged();
    void onBackwardKeyChanged();
    void onLeftKeyChanged();
    void onRightKeyChanged();

    void onScroll(double delta);

    void mousePress(double x, double y);
    void mouseRelease();
    void mouseMove(double x, double y);

    void update();

private:
    QVector3D _pos {0.f, 5.f, 10.f};
    QVector3D _front { 0.f, 0.f, -1.f};
    QVector3D _right {1.f, 0.f, 0.f};
    QVector3D _up {0.f, 1.f, 0.f};
    double _fov {45.0};

    bool _goingForward {false};
    bool _goingBackward {false};
    bool _goingLeft {false};
    bool _goingRight {false};

    bool _isMousePressed {false};
    double _lastX;
    double _lastY;

    double _yaw {-90.0};
    double _pitch {0.0};

};

#endif // CAMERA_H
