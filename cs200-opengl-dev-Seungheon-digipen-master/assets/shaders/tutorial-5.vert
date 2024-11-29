/*!************************************************************************
\file tutorial-5.vert
\author Seungheon Jeon, Rudy Castan
\par DP email: seungheon.jeon\@digipen.edu
\par Course: CS200/fall
\par CS200 Tutorial 5 - Texture MappingVirtual programming lab
\date 10-05-2023
\brief
This vertex shader specially get texture's coordinate and out this
**************************************************************************/

#version 410 core

layout(location = 0) in vec2 aVertexPosition;
layout(location = 1) in vec2 vTCoordinates;
layout(location = 2) in vec3 aVertexColor;

layout(location = 0) out vec3 vColor;
layout(location = 1) out vec2 vTextureCoordinates;

uniform float tex_coord_scale;


void main()
{
    vTextureCoordinates = vTCoordinates * tex_coord_scale;
    gl_Position = vec4(aVertexPosition, 0.0, 1.0);

    vColor      = aVertexColor;
}
