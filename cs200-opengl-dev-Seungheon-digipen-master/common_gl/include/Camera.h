/*!************************************************************************
\file Camera.h
\author Seungheon Jeon, Rudy Castan
\par DP email: seungheon.jeon\@digipen.edu
\par Course: CS200/fall
\par CS200 Tutorial 4 - Implementing 2D Interactive CameraVirtual programming lab
\date 09-29-2023
\brief
This header file declare Camera's member functions such as Get/Set functions and make 3X3 function
**************************************************************************/
#pragma once

#include "mat3.h"
#include "vec2.h"


class [[nodiscard]] Camera
{
public:
    vec2 Position{};
    bool IsFirstPerson{false};

public:
    Camera() noexcept = default;

    /*!
* \brief Constructor that get initial value
* \param vec2 camera_position
    position value
* \param vec2 camera_up
    up vector value
*/
    Camera(vec2 camera_position, vec2 camera_up) noexcept;


    [[nodiscard]] vec2 GetUp() const noexcept
    {
        return up;
    }

    [[nodiscard]] vec2 GetRight() const noexcept
    {
        return right;
    }

    [[nodiscard]] float GetOrientation() const noexcept
    {
        return orientation;
    }

    /*!
* \brief Set up vector and automatically compute right vector and orientation
* \param vec2 camera_up
    up vector value
*/
    void SetUp(vec2 camera_up = {0, 1}) noexcept;

    /*!
* \brief Set Orientation and automatically compute right vector and up vector
* \param float angle_radians
    angle_radians value
*/
    void SetOrientation(float angle_radians) noexcept;

    /*!
* \brief To move position to up and down
* \param float distance
    how much long
*/
    void MoveUp(float distance) noexcept;

    /*!
* \brief To move position to right and left
* \param float distance
    how much long
*/
    void MoveRight(float distance) noexcept;

    /*!
* \brief Set Orientation with parameter
* \param float angle_radians
    angle_radians value
*/
    void Rotate(float angle_radians) noexcept;

    /*!
* \brief Build CameraToWorld 3X3 matrix according to IsFirstPerson value and return this
*/
    mat3 BuildCameraToWorld() const noexcept;

    /*!
* \brief Build WorldToCamera 3X3 matrix according to IsFirstPerson value and return this
*/
    mat3 BuildWorldToCamera() const noexcept;

private:
    vec2  up{0, 1};
    vec2  right{1, 0};
    float orientation{0};
};
