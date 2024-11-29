/*!************************************************************************
\file Tutorial_5.cpp
\author Seungheon Jeon, Rudy Castan
\par DP email: seungheon.jeon\@digipen.edu
\par Course: CS200/fall
\par CS200 Tutorial 5 - Texture MappingVirtual programming lab
\date 10-05-2023
\brief
This source file make Tutorial_5 class and define function of this
**************************************************************************/

#include "Tutorial_5.h"

#include "angles.h"
#include "color3.h"
#include "glCheck.h"
#include "vec2.h"

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <array>
#include <cmath>
#include <fstream>
#include <imgui.h>
#include <iostream>
#include <string_view>
#include <Random.h>
using namespace util;

util::owner<IProgram*> create_program(int viewport_width, int viewport_height)
{
    return new Tutorial_5(viewport_width, viewport_height);
}

namespace
{
//#include "tutorial-5.frag.h"
//#include "tutorial-5.vert.h"
}

Tutorial_5::Tutorial_5(int viewport_width, int viewport_height)
{
    glCheck(glViewport(0, 0, viewport_width, viewport_height));
    
    shader = GLShader("Basic Shader",
        { {GLShader::VERTEX, "assets/shaders/tutorial-5.vert"},
        {GLShader::FRAGMENT, "assets/shaders/tutorial-5.frag"} });

    if (const bool loaded = duck_image.LoadFromFileImage("assets/images/duck.png"); !loaded)
    {
        throw std::runtime_error{ "Failed to load the duck.png" };
    }
    
    glCheck(glClearColor(0.0f, 0.68f, 0.94f, 1.0f));

    create_fullscreen_quad();

    timing.start_time = SDL_GetTicks64();
    timing.prev_time  = timing.start_time;
}

void Tutorial_5::Update()
{
    update_timing();

    if (settings.animate_procedural_texture) {
        float t = timing.reverseElapsedTime / animate_time;
        float e = (sin(PI<float>*t - HALF_PI<float>)+1) / 2.0f;
        settings.procedural_tile_size = Settings::MIN_TILE_SIZE + (e * (Settings::MAX_TILE_SIZE - Settings::MIN_TILE_SIZE));
    }

}

void Tutorial_5::Draw()
{
    glCheck(glClear(GL_COLOR_BUFFER_BIT));

    glCheck(shader.Use());

    glCheck(duck_image.UseForSlot(0));

    glCheck(shader.SendUniform("uTex2d", 0));
    
    glCheck(shader.SendUniform("uUseInterpolatedColor", settings.modulate_color));
    glCheck(shader.SendUniform("uUseProcedural_Color", settings.apply_procedural_texture));
    glCheck(shader.SendUniform("uUseTexture_Color", settings.use_texture));
    
    glCheck(shader.SendUniform("uTileSize", settings.procedural_tile_size));
    glCheck(shader.SendUniform("tex_coord_scale", settings.tex_coord_scale));

    glCheck(fullscreen_quad.Use());

    if (settings.do_blending) {
        glCheck(glEnable(GL_BLEND));
        glCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    }
    else {
        glCheck(glDisable(GL_BLEND));
    }

    glCheck(GLDrawIndexed(fullscreen_quad));

}

void Tutorial_5::ImGuiDraw()
{
    ImGui::Begin("Program Info");
    {
        ImGui::LabelText("FPS", "%.1f", double(timing.fps));

        ImGui::Checkbox("Modulate Color", &settings.modulate_color);
        ImGui::SameLine();
        ImGui::Checkbox("Procedural Texture", &settings.apply_procedural_texture);
        ImGui::SameLine();
        ImGui::Checkbox("Duck Texture", &settings.use_texture);
        ImGui::Separator();
        if (settings.apply_procedural_texture)
        {
            ImGui::Checkbox("Animate Procedural Size", &settings.animate_procedural_texture);

            if (ImGui::SliderFloat("Procedural Tile Size", &settings.procedural_tile_size, Settings::MIN_TILE_SIZE, Settings::MAX_TILE_SIZE, "%2.1f"))
            {
                settings.animate_procedural_texture = false;
            }
            ImGui::Separator();
        }
        if (settings.use_texture)
        {
            ImGui::Checkbox("Blending", &settings.do_blending);
            imgui_pick_filtering();
            ImGui::SliderFloat("Tex Coord Scale", &settings.tex_coord_scale, 0.25f, 10.0f, "%2.1f");
            imgui_pick_wrapping("Wrapping for S Direction", GLTexture::S);
            imgui_pick_wrapping("Wrapping for T Direction", GLTexture::T);
            ImGui::Separator();
        }
    }
    ImGui::End();
}

void Tutorial_5::HandleEvent(SDL_Window& sdl_window, const SDL_Event& event)
{
    switch (event.type)
    {
        case SDL_WINDOWEVENT:
        {
            switch (event.window.event)
            {
                case SDL_WINDOWEVENT_RESIZED:
                {
                    on_window_resized(sdl_window);
                }
                break;
                case SDL_WINDOWEVENT_SIZE_CHANGED:
                {
                    on_window_resized(sdl_window);
                }
                break;
            }
        }
    }
}

void Tutorial_5::on_window_resized(SDL_Window& sdl_window) noexcept
{
    int width = 0, height = 0;
    SDL_GL_GetDrawableSize(&sdl_window, &width, &height);
    glCheck(glViewport(0, 0, width, height));
}

void Tutorial_5::update_timing() noexcept
{
    const auto current_time = SDL_GetTicks64();
    const auto delta_ms     = current_time - timing.prev_time;
    timing.ElapsedTime += delta_ms / 1'000.0f;

    float num = delta_ms / 1'000.0f;
    if (timing.reverseElapsedTime > animate_time || timing.reverseElapsedTime < 0) {
        timing.check = !timing.check;
    }
    if (timing.check) {
        timing.reverseElapsedTime += num;
    }
    else {
        timing.reverseElapsedTime -= num;
    }

    timing.prev_time      = current_time;
    const auto elapsed_ms = current_time - timing.start_time;

    timing.count++;
    if (elapsed_ms > 1'000)
    {
        timing.fps        = timing.count / (elapsed_ms / 1'000.0f);
        timing.start_time = current_time;
        timing.count      = 0;
    }
}

void Tutorial_5::create_fullscreen_quad()
{
    std::vector<vec2>   pos_vtx{ vec2{ -1.0f, -1.0f }, vec2{ 1.0f, -1.0f }, vec2{ 1.0f, 1.0f }, vec2{ -1.0f, 1.0f } };
    std::vector<vec2>   tex_pos_vtx{ vec2{ 0.0f, 0.0f }, vec2{ 1.0f, 0.0f }, vec2{ 1.0f, 1.0f }, vec2{ 0.0f, 1.0f } };
    std::vector<color3> clr_vtx{ color3{ 1, 0, 0 }, color3{ 0, 1, 0 }, color3{ 0, 0, 1 }, color3{ 1, 0, 1 } };
    std::vector<unsigned> indices{3,0,2,1};

    GLsizei       positions_byte_size = static_cast<GLsizei>(sizeof(vec2) * pos_vtx.size());
    GLsizei       tex_positions_byte_size = static_cast<GLsizei>(sizeof(vec2) * tex_pos_vtx.size());
    GLsizei       colors_byte_size = static_cast<GLsizei>(sizeof(color3) * clr_vtx.size());

    GLsizei       buffer_size = positions_byte_size + colors_byte_size + tex_positions_byte_size;
    GLVertexBuffer       buffer(buffer_size);
    glCheck(buffer.SetData(std::span(pos_vtx)));
    glCheck(buffer.SetData(std::span(tex_pos_vtx), positions_byte_size));
    glCheck(buffer.SetData(std::span(clr_vtx), positions_byte_size + tex_positions_byte_size));

    GLAttributeLayout position;
    position.component_type = GLAttributeLayout::Float;
    position.component_dimension = GLAttributeLayout::_2;
    position.normalized = false;
    position.vertex_layout_location = 0; // 1st field is 0 index based
    position.stride = sizeof(vec2);
    position.offset = 0;
    position.relative_offset = 0;

    GLAttributeLayout tex_position;
    tex_position.component_type = GLAttributeLayout::Float;
    tex_position.component_dimension = GLAttributeLayout::_2;
    tex_position.normalized = false;
    tex_position.vertex_layout_location = 1; // 1st field is 0 index based
    tex_position.stride = sizeof(vec2);
    tex_position.offset = positions_byte_size;
    tex_position.relative_offset = 0;
    
    GLAttributeLayout color;
    color.component_type = GLAttributeLayout::Float;
    color.component_dimension = GLAttributeLayout::_3;
    color.normalized = false;
    color.vertex_layout_location = 2; // 2nd field of Vertex
    color.stride = sizeof(color3);
    color.offset = positions_byte_size + tex_positions_byte_size; // starts after the position bytes
    color.relative_offset = 0;
    
    glCheck(fullscreen_quad.AddVertexBuffer(std::move(buffer), { position, tex_position, color }));
    glCheck(fullscreen_quad.SetPrimitivePattern(GLPrimitive::TriangleStrip));

    GLIndexBuffer       index_buffer(indices);
    glCheck(fullscreen_quad.SetIndexBuffer(std::move(index_buffer)));
}

void Tutorial_5::imgui_pick_filtering()
{
    constexpr const char*          items[]             = {"NearestPixel", "Linear"};
    constexpr GLTexture::Filtering types[]             = {GLTexture::NearestPixel, GLTexture::Linear};
    const char* const              combo_preview_value = items[settings.filtering_index];
    if (ImGui::BeginCombo("Texture Filtering", combo_preview_value, 0))
    {
        for (int n = 0; n < IM_ARRAYSIZE(items); n++)
        {
            const bool is_selected = (settings.filtering_index == n);
            if (ImGui::Selectable(items[n], is_selected))
            {
                settings.filtering_index = n;
                duck_image.SetFiltering(types[n]);
            }
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
}

void Tutorial_5::imgui_pick_wrapping(const char* label, GLTexture::Coordinate which_coord)
{
    constexpr const char*         items[] = {"Repeat", "ClampToEdge", "ClampToBorder", "MirroredRepeat", "MirrorClampToEdge"};
    constexpr GLTexture::Wrapping types[] = {GLTexture::Repeat, GLTexture::ClampToEdge, GLTexture::ClampToBorder, GLTexture::MirroredRepeat, GLTexture::MirrorClampToEdge};
    const char* const             combo_preview_value = items[settings.wrap_index[which_coord]];

    if (ImGui::BeginCombo(label, combo_preview_value, 0))
    {
        for (int n = 0; n < IM_ARRAYSIZE(items); n++)
        {
            const bool is_selected = (settings.wrap_index[which_coord] == n);
            if (ImGui::Selectable(items[n], is_selected))
            {
                settings.wrap_index[which_coord] = n;
                duck_image.SetWrapping(types[n], which_coord);
            }
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
}
