/*!************************************************************************
\file model_to_ndc_pos2d_clr.vert
\author Seungheon Jeon, Rudy Castan
\par DP email: seungheon.jeon\@digipen.edu
\par Course: CS200/fall
\par CS200 Tutorials 3 - Geometric Instancing Using Model TransformsVirtual programming lab
\date 09-17-2023
\brief
This shader file has a uniform value calculate matrix
**************************************************************************/

#version 450 core

layout (location=0) in  vec2 aVertexPosition;
layout (location=1) in  vec3 aVertexColor;
layout (location=0) out vec3 vColor;

uniform mat3 uModelToNDC;

void main() {
  gl_Position = vec4(vec2(uModelToNDC * vec3(aVertexPosition, 1.f)), 
                     0.0, 1.0);
  vColor = aVertexColor;
}