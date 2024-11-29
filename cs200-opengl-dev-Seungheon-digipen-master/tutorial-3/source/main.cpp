/*!************************************************************************
\file main.cpp
\author Seungheon Jeon, Rudy Castan
\par DP email: seungheon.jeon\@digipen.edu
\par Course: CS200/fall
\par CS200 Tutorials 3 - Geometric Instancing Using Model TransformsVirtual programming lab
\date 09-17-2023
\brief
This source file is main, nothing special
**************************************************************************/

#include "GLApp.h"

#include <iostream>

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
try
{
    GLApp OpenGLApplication("Tutorial 3 for Seungheon Jeon: Geometric Instancing Using Model Transforms", 640, 640);

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
