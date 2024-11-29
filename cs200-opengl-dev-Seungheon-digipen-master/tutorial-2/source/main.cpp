/*!************************************************************************
\file main.cpp
\author Seungheon Jeon, Rudy Castan
\par DP email: seungheon.jeon\@digipen.edu
\par Course: CS200/fall
\par CS200 Tutorial 2 - Viewports and OpenGL Primitives
\date 09-13-2023
\brief
This source file is main, nothing special
**************************************************************************/

#include "GLApp.h"

#include <iostream>

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
try
{
    // TODO: Update name
    GLApp OpenGLApplication("Tutorial 2 for Seungheon Jeon: Viewport and Primitives");

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
