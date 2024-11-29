/*!************************************************************************
\file tutorial-5.frag
\author Seungheon Jeon, Rudy Castan
\par DP email: seungheon.jeon\@digipen.edu
\par Course: CS200/fall
\par CS200 Tutorial 5 - Texture MappingVirtual programming lab
\date 10-05-2023
\brief
This fragment shader interpolate several color such as procedural_color of check patterns, texture color and original polygon's color
**************************************************************************/

#version 450 core

layout(location = 0) in vec3 vColor;
layout(location = 1) in vec2 vTextureCoordinates;

layout(location = 0) out vec4 fFragClr;

uniform float uTileSize;

uniform bool uUseInterpolatedColor;
uniform bool uUseProcedural_Color;
uniform bool uUseTexture_Color;

uniform sampler2D uTex2d;

void main()
{
    vec3 gridColor = vec3(1, 0, 1);
    vec3 gridColor2 = vec3(0.0,0.68,0.94);
    
    vec2 tileCoord = vec2(gl_FragCoord.xy) / uTileSize;

vec4 interpolated_color = vec4(1);
    if(uUseInterpolatedColor) 
        interpolated_color = vec4(vColor, 1.0);

vec4 procedural_color = vec4(1);
    if(uUseProcedural_Color) {
        if (mod(floor(tileCoord.x) + floor(tileCoord.y), 2.0) == 0) {
            procedural_color = vec4(gridColor, 1.0);
        } else {
            procedural_color = vec4(gridColor2, 1.0);
        }
    }

vec4 texture_color = vec4(1);
    if(uUseTexture_Color) 
        texture_color = texture(uTex2d, vTextureCoordinates);

    fFragClr = interpolated_color * procedural_color * texture_color;
}