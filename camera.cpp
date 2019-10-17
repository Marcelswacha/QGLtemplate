#include "camera.h"

#include <QDebug>
#include <QtMath>

Camera::Camera(QObject *parent) : QObject(parent)
{

}

void Camera::onForwardKeyChanged()
{
    goingForward = !goingForward;
}

void Camera::onBackwardKeyChanged()
{
    goingBackward = !goingBackward;
}

void Camera::onLeftKeyChanged()
{
    goingLeft = !goingLeft;
}

void Camera::onRightKeyChanged()
{
    goingRight = !goingRight;
}

void Camera::update()
{
    float velocity = 0.15f;
    if (goingForward) {
        pos += front * velocity;
    }
    if (goingBackward) {
        pos -= front * velocity;
    }
    if (goingLeft) {
        pos -= right * velocity;
    }
    if (goingRight) {
        pos += right * velocity;
    }
}

QMatrix4x4 Camera::view()
{
    QMatrix4x4 v;
    v.lookAt(pos, pos + front, up);

    return v;
}

void Camera::mousePress(double x, double y)
{
    isMousePressed = true;
    lastX = x;
    lastY = y;
}

void Camera::mouseRelease()
{
    isMousePressed = false;
}

void Camera::mouseMove(double x, double y)
{
    if (!isMousePressed) {
        return;
    }

    double xoffset = x - lastX;
    double yoffset = lastY - y;
    lastX = x;
    lastY = y;

    double sensitivity = 25.0;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    if(pitch > 89.0)
        pitch = 89.0;
    if(pitch < -89.0)
        pitch = -89.0;

    QVector3D newFront;
    newFront.setX( cos(qDegreesToRadians(yaw)) * cos(qDegreesToRadians(pitch)) );
    newFront.setY( sin(qDegreesToRadians(pitch)) );
    newFront.setZ( sin(qDegreesToRadians(yaw)) * cos(qDegreesToRadians(pitch)) );
    newFront.normalize();
    front = newFront;
}


