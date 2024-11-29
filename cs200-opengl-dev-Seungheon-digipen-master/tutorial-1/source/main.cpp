/*!************************************************************************
\file main.cpp
\author Seungheon Jeon, Rudy Castan
\par DP email: seungheon.jeon\@digipen.edu
\par Course: CS200/fall
\par CS200 Tutorial 1 - Initializing OpenGL Context
\date 09-09-2023
\brief
main source file, contain name and create object
**************************************************************************/

#include "GLApp.h"

#include <iostream>

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
try
{
    GLApp OpenGLApplication("Tutorial 1 for Seungheon Jeon: Intro OpenGL");

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
