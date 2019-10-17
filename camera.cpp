#include "camera.h"

#include <QDebug>
#include <QtMath>

Camera::Camera(QObject *parent) : QObject(parent)
{
}

void Camera::onForwardKeyChanged()
{
    _goingForward = !_goingForward;
}

void Camera::onBackwardKeyChanged()
{
    _goingBackward = !_goingBackward;
}

void Camera::onLeftKeyChanged()
{
    _goingLeft = !_goingLeft;
}

void Camera::onRightKeyChanged()
{
    _goingRight = !_goingRight;
}

void Camera::onScroll(double delta)
{
    _fov += delta / -20.0;
    if (_fov < 1.0) {
        _fov = 1.0;
    }
    if (_fov > 120.0) {
        _fov = 120;
    }
}

void Camera::update()
{
    float velocity = 0.15f;
    if (_goingForward) {
        _pos += _front * velocity;
    }
    if (_goingBackward) {
        _pos -= _front * velocity;
    }
    if (_goingLeft) {
        _pos -= _right * velocity;
    }
    if (_goingRight) {
        _pos += _right * velocity;
    }
}

QMatrix4x4 Camera::view()
{
    QMatrix4x4 v;
    v.lookAt(_pos, _pos + _front, _up);

    return v;
}

void Camera::mousePress(double x, double y)
{
    _isMousePressed = true;
    _lastX = x;
    _lastY = y;
}

void Camera::mouseRelease()
{
    _isMousePressed = false;
}

void Camera::mouseMove(double x, double y)
{
    if (!_isMousePressed) {
        return;
    }

    double xoffset = x - _lastX;
    double yoffset = _lastY - y;
    _lastX = x;
    _lastY = y;

    double sensitivity = 25.0;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    _yaw   += xoffset;
    _pitch += yoffset;

    if(_pitch > 89.0)
        _pitch = 89.0;
    if(_pitch < -89.0)
        _pitch = -89.0;

    QVector3D newFront;
    newFront.setX( cos(qDegreesToRadians(_yaw)) * cos(qDegreesToRadians(_pitch)) );
    newFront.setY( sin(qDegreesToRadians(_pitch)) );
    newFront.setZ( sin(qDegreesToRadians(_yaw)) * cos(qDegreesToRadians(_pitch)) );
    newFront.normalize();
    _front = newFront;
}


