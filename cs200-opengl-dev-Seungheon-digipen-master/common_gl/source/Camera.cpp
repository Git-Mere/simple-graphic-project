/*!************************************************************************
\file Camera.cpp
\author Seungheon Jeon, Rudy Castan
\par DP email: seungheon.jeon\@digipen.edu
\par Course: CS200/fall
\par CS200 Tutorial 4 - Implementing 2D Interactive CameraVirtual programming lab
\date 09-29-2023
\brief
This source file implement Camera's member functions such as Get/Set functions and make 3X3 function
**************************************************************************/

#include "Camera.h"
#include <corecrt_math.h>

Camera::Camera(vec2 camera_position, vec2 camera_up) noexcept : Position(camera_position), IsFirstPerson(false),
up(camera_up), right(perpendicular_to(up)), orientation(static_cast<float>(atan2(up.y, up.x)))
{
    // Position <- camera_position
    // IsFirstPerson <- false
    // up <- camera_up
    // right <- Rot -90(up)
    // orientation <- angle representation of up vector
}

void Camera::SetUp(vec2 camera_up) noexcept
{
    up = camera_up;
    right = perpendicular_to(up);
    orientation = static_cast<float>(atan2(up.y, up.x));
    // up <- camera_up
    // right <- Rot -90(up)
    // orientation <- angle representation of up vector
}

void Camera::SetOrientation(float angle_radians) noexcept
{
    orientation = angle_radians;
    up = vec2{static_cast<float>(cos(orientation)), static_cast<float>(sin(orientation))};
    right = perpendicular_to(up);
    // orientation <- angle_radians
    // up <- vector from angle orientation
    // right < -Rot - 90(up)
}

void Camera::MoveUp(float distance) noexcept
{
    Position = Position + up * distance;
}

void Camera::MoveRight(float distance) noexcept
{
    Position = Position + right * distance;
}

void Camera::Rotate(float angle_radians) noexcept
{
    SetOrientation(orientation + angle_radians);
}

mat3 Camera::BuildCameraToWorld() const noexcept
{
    if (IsFirstPerson)
    {
        return { right.x, right.y, 0.0f, up.x, up.y, 0.0f, Position.x, Position.y, 1.0f };
        // build and return camera to world matrix using our up, right and position vectors
    }
    return { 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, Position.x, Position.y, 1.0f };
    // build and return camera to world matrix using only our position vector

}

mat3 Camera::BuildWorldToCamera() const noexcept
{
    if (IsFirstPerson)
    {
        return { right.x, up.x, 0.0f, right.y, up.y, 0.0f, -dot(Position, right), -dot(Position, up), 1.0f };
        // build and return world to camera matrix using our up, right and position vectors
    }
    return { 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, dot(Position, vec2{-1.0f, 0.0f}), dot(Position, vec2{0.0f, -1.0f}), 1.0f};
    // build and return camera to world matrix using only our position vector
}
