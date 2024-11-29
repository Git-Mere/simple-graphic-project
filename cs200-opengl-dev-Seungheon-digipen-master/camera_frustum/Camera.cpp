/*!************************************************************************
\file Camera.cpp
\author Seungheon Jeon
\par DP email: seungheon.jeon\@digipen.edu
\par Course: CS200/fall
\par Camera Frustum Programming Assignment
\date 11-10-2023
\brief
Implement function about SimpleRender
**************************************************************************/

#include "Camera.h"

Camera::Camera(void)
    : eye(Point(0, 0, 0)), right(Vector(1, 0, 0)), up(Vector(0, 1, 0)), back(Vector(0, 0, -1)),
    width(1), height(1), distance(1), near(0.1f), far(10) {}

/*!*****************************************************************************
\brief
    based on several parameter, determin back, right, up vector and width, height
    I set dinstance is between near and far
\param E
    eye
\param look
    look vector
\param rel
    relative up vector
\param fov
    angle
\param aspect
    aspect
\param near
    near
\param far
    far
\return
  result Hcoord
*******************************************************************************/
Camera::Camera(const Point& E, const Vector& look, const Vector& rel, float fov, float aspect, float near, float far)
    : eye(E), width(1), height(1), distance((far - near) / 2), near(near), far(far) {

    back = -(1.f / abs(look) * look);
    right = (1 / abs(cross(look, rel))) * cross(look, rel);
    up = cross(back, right);

    width = 2 * std::tan(fov / 2) * distance;
    height = width / aspect;
}

Point Camera::Eye(void) const {
    return eye;
}

Vector Camera::Right(void) const {
    return normalize(right);
}

Vector Camera::Up(void) const {
    return normalize(up);
}

Vector Camera::Back(void) const {
    return normalize(back);
}

Vector Camera::ViewportGeometry(void) const {
    return Vector(width, height, distance);
}

float Camera::NearDistance(void) const {
    return  near;
}

float Camera::FarDistance(void) const {
    return far;
}

/*!*****************************************************************************
\brief
    Cameara zoom
\param factor
    zoom factor
\return
    *this
*******************************************************************************/
Camera& Camera::Zoom(float factor) {
    width *= factor;
    height *= factor;
    return *this;
}

/*!*****************************************************************************
\brief
    forward eye
\param distance_increment
    how long
\return
    *this
*******************************************************************************/
Camera& Camera::Forward(float distance_increment) {
    eye = eye + (distance_increment * -back);
    return *this;
}

/*!*****************************************************************************
\brief
    Rotate Yaw axis
\param angle
    value how much rotate
\return
  *this
*******************************************************************************/
Camera& Camera::Yaw(float angle) {
    Vector newRight = std::cos(angle) * right + std::sin(angle) * -back;
    Vector newBack = cross(newRight, up);
    right = newRight;
    back = newBack;
    return *this;

}

/*!*****************************************************************************
\brief
    Rotate Pitch axis
\param angle
    value how much rotate
\return
  *this
*******************************************************************************/
Camera& Camera::Pitch(float angle) {
    Vector newUp = std::cos(angle) * up  + std::sin(angle) * back;
    Vector newBack = cross(right, newUp);
    up = newUp;
    back = newBack;
    return *this;
}

/*!*****************************************************************************
\brief
    Rotate Roll axis
\param angle
    value how much rotate
\return
  *this
*******************************************************************************/
Camera& Camera::Roll(float angle) {
    Vector newRight = std::cos(angle) * right + std::sin(angle) * up;
    Vector newUp = cross(back, newRight);
    right = newRight;
    up = newUp;
    return *this;
}

Camera& Camera::MoveRight(float distance_increment) {
    eye = eye + distance_increment * right;
    return *this;
}

Camera& Camera::MoveLeft(float distance_increment) {
    eye = eye + distance_increment * -right;
    return *this;
}