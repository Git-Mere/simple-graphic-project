/*!************************************************************************
\file CameraView.h
\author Seungheon Jeon, Rudy Castan
\par DP email: seungheon.jeon\@digipen.edu
\par Course: CS200/fall
\par CS200 Tutorial 4 - Implementing 2D Interactive CameraVirtual programming lab
\date 09-29-2023
\brief
This header file declare CameraView's member functions such as Get/Set functions and make 3X3 function
**************************************************************************/
#pragma once
#include "mat3.h"
#include "vec2.h"

class [[nodiscard]] CameraView
{
public:
    /*!
* \brief Set FramebufferSize
* \param vec2 the_framebuffer_size 
    vec2 value
*/
    void SetFramebufferSize(vec2 the_framebuffer_size) noexcept;

    /*!
* \brief Set FramebufferSize
* \param int new_framebuffer_width
    width value
* \param int new_framebuffer_height
    height value
*/
    void SetFramebufferSize(int new_framebuffer_width, int new_framebuffer_height) noexcept;

    /*!
* \brief Get ViewSize
* \return framebuffer_size
*/
    vec2 GetViewSize() const;

    /*!
* \brief Set Zoom
* \param float new_zoom
    value
*/
    void SetZoom(float new_zoom) noexcept;

    vec2 CalcViewSizeWithZoom() const noexcept
    {
        return framebuffer_size / zoom;
    }

    [[nodiscard]] float GetZoom() const noexcept
    {
        return zoom;
    }

    /*!
* \brief Build CameraToNDC 3X3 matrix
* \return result matrix
*/
    mat3 BuildCameraToNDC() const noexcept;

    /*!
* \brief Build CameraToNDC 3X3 matrix
* \return result matrix
*/
    mat3 BuildWindowDeviceToCamera() const noexcept;

private:
    vec2  framebuffer_size{};
    float zoom{1.0f};
};
