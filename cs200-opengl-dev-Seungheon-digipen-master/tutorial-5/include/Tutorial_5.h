/*!************************************************************************
\file Tutorial_5.h
\author Seungheon Jeon, Rudy Castan
\par DP email: seungheon.jeon\@digipen.edu
\par Course: CS200/fall
\par CS200 Tutorial 5 - Texture MappingVirtual programming lab
\date 10-05-2023
\brief
This header file make Tutorial_5 class and declare functions of this
**************************************************************************/

#pragma once

#include "GLShader.h"
#include "GLTexture.h"
#include "GLVertexArray.h"
#include "IProgram.h"

class Tutorial_5 : public IProgram
{
public:
    Tutorial_5(int viewport_width, int viewport_height);

    void Update() override;
    /*!
* \brief draw object
*/
    void Draw() override;
    void ImGuiDraw() override;
    void HandleEvent(SDL_Window& sdl_window, const SDL_Event& event) override;

private:
    struct
    {
        unsigned long long prev_time   = 0;
        unsigned long long start_time  = 0;
        float              fps         = 0;
        float              ElapsedTime = 0;
        float              reverseElapsedTime = 0;
        bool               check = false;
        int                count = 0;
    } timing;

    static constexpr float animate_time = 10.0f;

    GLVertexArray fullscreen_quad;
    GLShader      shader;
    GLTexture     duck_image;

    struct Settings
    {
        int                   filtering_index            = 0;
        std::array<int, 2>    wrap_index                 = {0, 0};
        float                 tex_coord_scale            = 1.0f;
        bool                  modulate_color             = false;
        bool                  apply_procedural_texture   = false;
        float                 procedural_tile_size       = 8.0f;
        bool                  use_texture                = true;
        bool                  do_blending                = false;
        bool                  animate_procedural_texture = false;
        static constexpr auto MAX_TILE_SIZE              = 128.0f;
        static constexpr auto MIN_TILE_SIZE              = 4.0f;
    } settings;


private:
    void on_window_resized(SDL_Window& sdl_window) noexcept;
    void update_timing() noexcept;
    /*!
* \brief create fullscreen_quad VOA object (define texel coordinate)
*/
    void create_fullscreen_quad();
    void imgui_pick_filtering();
    void imgui_pick_wrapping(const char* label, GLTexture::Coordinate which_coord);
};
