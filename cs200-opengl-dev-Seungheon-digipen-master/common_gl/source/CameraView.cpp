/*!************************************************************************
\file CameraView.cpp
\author Seungheon Jeon, Rudy Castan
\par DP email: seungheon.jeon\@digipen.edu
\par Course: CS200/fall
\par CS200 Tutorial 4 - Implementing 2D Interactive CameraVirtual programming lab
\date 09-29-2023
\brief
This source file implement CameraView's member functions such as Get/Set functions and make 3X3 function
**************************************************************************/
#include "CameraView.h"

void CameraView::SetFramebufferSize(vec2 the_framebuffer_size) noexcept
{
	framebuffer_size = the_framebuffer_size;
}

void CameraView::SetFramebufferSize(int new_pixel_width, int new_pixel_height) noexcept
{
	SetFramebufferSize(vec2{ float(new_pixel_width), float(new_pixel_height) });
}

vec2 CameraView::GetViewSize() const
{
    return framebuffer_size;
}

void CameraView::SetZoom(float new_zoom) noexcept
{
    if (new_zoom != 0) {
        zoom = new_zoom;
    }
}

mat3 CameraView::BuildCameraToNDC() const noexcept
{
    vec2 value = CalcViewSizeWithZoom();
    return { 2/value.x, 0.0f, 0.0f, 0.0f, 2 / value.y, 0.0f, 0.0f, 0.0f, 1.0f };
    //         (v.w/2,v.h/2)              (1,1)
    //      +--------o             +--------o
    //      |        |             |        |
    //      | cam    |     --->    | ndc    |
    //      o--------+             o--------+
    // (-v.w/2,-v.h/2)          (-1,-1)
    //

    // get view size with zoom
    // build and return camera to NDC matrix
}

mat3 CameraView::BuildWindowDeviceToCamera() const noexcept
{
    vec2 value = CalcViewSizeWithZoom();
    vec2 value2 = GetViewSize();
    return { value.x / value2.x, 0.0f, 0.0f, 0.0f, value.y / -value2.y, 0.0f, value.x / -2, value.y / 2, 1.0f };
    //    (0,0)             (-v.w/2,v.h/2)
    //      o--------+             o--------+
    //      |        |             |        |
    //      | win    |     --->    | cam    |
    //      +--------o             +--------o
    //           (f.w,f.h)            (v.w/2,-v.h/2)
    //

    // get view size with zoom
    // build and return window to camera matrix
}
