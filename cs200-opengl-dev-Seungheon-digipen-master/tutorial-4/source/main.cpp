/*!************************************************************************
\file main.cpp
\author Seungheon Jeon, Rudy Castan
\par DP email: seungheon.jeon\@digipen.edu
\par Course: CS200/fall
\par CS200 Tutorial 4 - Implementing 2D Interactive CameraVirtual programming lab
\date 09-29-2023
\brief
This source is main. nothing special
**************************************************************************/

#include "GLApp.h"

#include <iostream>

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
try
{
    // TODO: Update name
    GLApp OpenGLApplication("Tutorial 4 for Seungheon Jeon: 2D Camera & Affine Transformations");

    while (!OpenGLApplication.IsDone())
    {
        OpenGLApplication.Update();
    }

    return 0;
}
catch (const std::exception& e)
{
    std::cerr << e.what() << '\n';
    return -1;
}
