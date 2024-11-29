/*!************************************************************************
\file Tutorial_1.cpp
\author Seungheon Jeon, Rudy Castan
\par DP email: seungheon.jeon\@digipen.edu
\par Course: CS200/fall
\par CS200 Tutorial 1 - Initializing OpenGL Context
\date 09-09-2023
\brief
This source file implement Tutorial_1's member functions and several functions.
**************************************************************************/

#include "Tutorial_1.h"

#include "color3.h"
#include "glCheck.h"
#include "vec2.h"

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <array>
#include <imgui.h>
#include <iostream>

util::owner<IProgram*> create_program(int viewport_width, int viewport_height)
{
    return new Tutorial_1(viewport_width, viewport_height);
}

Tutorial_1::Tutorial_1(int viewport_width, int viewport_height)
{
    // Use entire window as viewport ...
    glCheck(glViewport(0, 0, viewport_width, viewport_height));
    // clear colorbuffer with RGBA value for Cornflower Blue in glClearColor ...
    glCheck(glClearColor(0.392f, 0.584f, 0.929f, 1));

    print_and_save_opengl_settings();

    shader = GLShader("Basic Shader",
        { {GLShader::VERTEX, "assets/shaders/pass_thru_pos2d_clr.vert"},
        {GLShader::FRAGMENT, "assets/shaders/basic_vtx_clr_attribute.frag"} });

    create_1buffer_struct_of_arrays_style();
    create_1buffer_array_of_structs_style();
    create_parallel_buffers_style();

    timing.start_time = SDL_GetTicks64();
    timing.prev_time  = timing.start_time;
}

void Tutorial_1::Update()
{
    update_timing();

    set_clear_color();
    
}


void Tutorial_1::Draw()
{
    glCheck(glClearColor(background.red, background.green, background.blue, 1));

    glCheck(glClear(GL_COLOR_BUFFER_BIT));
    shader.Use();

    left_eye_model.Use();
    GLDrawIndexed(left_eye_model);
    left_eye_model.Use(false);

    right_eye_model.Use();
    GLDrawIndexed(right_eye_model);
    right_eye_model.Use(false);

    mouth_model.Use();
    GLDrawIndexed(mouth_model);
    mouth_model.Use(false);

    shader.Use(false);
}

void Tutorial_1::set_clear_color()
{
    if (color_count == 2) { background.red += 0.005f;}
    else if (color_count == 3) { background.green -= 0.005f;}
    else if (color_count == 4) { background.blue += 0.005f;}
    else if (color_count == 5) { background.red -= 0.005f; }
    else if (color_count == 6) { background.green += 0.005f; }
    else if (color_count == 7) { background.blue -= 0.005f; }
    else if (color_count > 7) {  color_count = 1; }
}

void Tutorial_1::ImGuiDraw()
{
    ImGui::Begin("Program Info");
    {
        ImGui::LabelText("FPS", "%.1f", timing.fps);

        for (const auto& [label, description] : settings_descriptions)
        {
            ImGui::LabelText(label, "%s", description);
        }
        for (const auto& [label, description] : settings_descriptions2)
        {
            ImGui::LabelText(label, "%d", description);
        }

        view_dims.second[0] = store[0];
        view_dims.second[1] = store[1];
        ImGui::LabelText(view_dims.first, "%d x %d", view_dims.second[0], view_dims.second[1]);
        ImGui::LabelText(bul.first, "%s", bul.second ? "True" : "False");
        {
            ImGui::LabelText("Background Color", "RGB(%.0f,%.0f,%.0f)", background.red * 255, background.green * 255, background.blue * 255);
        }
    }
    ImGui::End();
}

void Tutorial_1::HandleEvent(SDL_Window& sdl_window, const SDL_Event& event)
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

void Tutorial_1::create_1buffer_struct_of_arrays_style()
{
    constexpr std::array positions           = {vec2{-0.2f, 0.2f}, vec2{-0.2f, 0.6f}, vec2{-0.6f, 0.6f}, vec2{-0.6f, 0.2f}};
    constexpr auto       positions_byte_size = static_cast<long long>(sizeof(vec2) * positions.size());
    constexpr std::array colors              = {color3{1, 1, 1}, color3{1, 0, 0}, color3{0, 1, 0}, color3{0, 0, 1}};
    constexpr auto       colors_byte_size    = static_cast<long long>(sizeof(color3) * colors.size());
    constexpr auto       buffer_size         = positions_byte_size + colors_byte_size;
    constexpr std::array<unsigned, 6> indices = {0, 1, 2, 2, 3, 0};

    GLVertexBuffer       buffer(buffer_size);
    buffer.SetData(std::span(positions));
    buffer.SetData(std::span(colors), positions_byte_size);

    GLAttributeLayout position;
    position.component_type = GLAttributeLayout::Float;
    position.component_dimension = GLAttributeLayout::_2;
    position.normalized = false;
    position.vertex_layout_location = 0; // 1st field is 0 index based
    position.stride = sizeof(vec2);
    position.offset = 0;
    position.relative_offset = 0;

    GLAttributeLayout color;
    color.component_type = GLAttributeLayout::Float;
    color.component_dimension = GLAttributeLayout::_3;
    color.normalized = false;
    color.vertex_layout_location = 1; // 2nd field of Vertex
    color.stride = sizeof(color3);
    color.offset = positions_byte_size; // starts after the position bytes
    color.relative_offset = 0;

    left_eye_model.AddVertexBuffer(std::move(buffer), { position, color });
    left_eye_model.SetPrimitivePattern(GLPrimitive::Triangles);

    GLIndexBuffer                     index_buffer(indices);
    left_eye_model.SetIndexBuffer(std::move(index_buffer));
}

namespace
{
    struct Vertex
    {
        vec2   position{};
        color3 color{};
    };

}

void Tutorial_1::create_1buffer_array_of_structs_style()
{
    constexpr std::array vertices = {Vertex{vec2{0.6f, 0.2f}, color3{1, 0, 0}}, Vertex{vec2{0.6f, 0.6f}, color3{0, 1, 0}}, Vertex{vec2{0.2f, 0.6f}, color3{0, 0, 1}},
                                     Vertex{vec2{0.2f, 0.2f}, color3{1, 1, 1}}};
    constexpr std::array<unsigned short, 6> indices = {0, 1, 2, 2, 3, 0};
    
    GLVertexBuffer buffer(std::span{ vertices });

    GLAttributeLayout position;
    position.component_type = GLAttributeLayout::Float;
    position.component_dimension = GLAttributeLayout::_2;
    position.normalized = false;
    position.vertex_layout_location = 0; // 1st field is 0 index based
    position.stride = sizeof(Vertex);
    position.offset = 0;
    position.relative_offset = offsetof(Vertex, position);

    GLAttributeLayout color;
    color.component_type = GLAttributeLayout::Float;
    color.component_dimension = GLAttributeLayout::_3;
    color.normalized = false;
    color.vertex_layout_location = 1; // 2nd field of Vertex
    color.stride = sizeof(Vertex);
    color.offset = 0; 
    color.relative_offset = offsetof(Vertex, color);

    right_eye_model.AddVertexBuffer(std::move(buffer), { position, color });
    right_eye_model.SetPrimitivePattern(GLPrimitive::Triangles);

    GLIndexBuffer                     index_buffer(indices);
    right_eye_model.SetIndexBuffer(std::move(index_buffer));
}

void Tutorial_1::create_parallel_buffers_style()
{
    constexpr std::array positions = {vec2{0.6f, -0.6f}, vec2{0.6f, -0.2f}, vec2{-0.6f, -0.2f}, vec2{-0.6f, -0.6f}};
    constexpr std::array colors    = {color3{0, 0, 1}, color3{1, 1, 1}, color3{1, 0, 0}, color3{0, 1, 0}};
    constexpr std::array<unsigned char, 6> indices = {0, 1, 2, 2, 3, 0};

    GLVertexBuffer    position_buffer(std::span{ positions });
    GLVertexBuffer    colors_buffer(std::span{ colors });
    
    GLAttributeLayout position;
    position.component_type = GLAttributeLayout::Float;
    position.component_dimension = GLAttributeLayout::_2;
    position.normalized = false;
    position.vertex_layout_location = 0; // 1st field is 0 index based
    position.stride = sizeof(vec2);
    position.offset = 0;
    position.relative_offset = 0;

    GLAttributeLayout color;
    color.component_type = GLAttributeLayout::Float;
    color.component_dimension = GLAttributeLayout::_3;
    color.normalized = false;
    color.vertex_layout_location = 1; // 2nd field of Vertex
    color.stride = sizeof(color3);
    color.offset = 0; // starts after the position bytes
    color.relative_offset = 0;

    mouth_model.AddVertexBuffer(std::move(position_buffer), { position });
    mouth_model.AddVertexBuffer(std::move(colors_buffer), { color });

    mouth_model.SetPrimitivePattern(GLPrimitive::Triangles);
    GLIndexBuffer                     index_buffer(indices);
    mouth_model.SetIndexBuffer(std::move(index_buffer));
}

void Tutorial_1::on_window_resized(SDL_Window& sdl_window) noexcept
{
    int width = 0, height = 0;
    SDL_GL_GetDrawableSize(&sdl_window, &width, &height);
    glCheck(glViewport(0, 0, width, height));
}

void Tutorial_1::update_timing() noexcept
{
    const auto current_time = SDL_GetTicks64();

    timing.prev_time      = current_time;
    const auto elapsed_ms = current_time - timing.start_time;

    timing.count++;
    if (elapsed_ms > 1'000)
    {
        color_count += 1;
        timing.fps        = timing.count / (elapsed_ms / 1'000.0);
        timing.start_time = current_time;
        timing.count      = 0;
    }
}

void Tutorial_1::print_and_save_opengl_settings() noexcept
{
    glCheck(GLubyte const* str_ven{ glGetString(GL_VENDOR) });
    glCheck(GLubyte const* str_ren = glGetString(GL_RENDERER));
    glCheck(GLubyte const* str_ver = glGetString(GL_VERSION));
    glCheck(GLubyte const* str_sver = glGetString(GL_SHADING_LANGUAGE_VERSION));

    settings_descriptions.push_back({ "GL_VENDOR", str_ven });
    settings_descriptions.push_back({ "GL_RENDERER", str_ren });
    settings_descriptions.push_back({ "GL_VERSION", str_ver });
    settings_descriptions.push_back({ "GL_SHADING_LANGUAGE_VERSION", str_sver });

    GLint major_ver;
    GLint minor_ver;
    GLint elements_vertices;
    GLint elemetns_indices;
    GLint texture_image_units;
    GLint tex_size;
    GLint viewport_dims[2];
    GLboolean doublebuffer;

    glCheck(glGetIntegerv(GL_MAJOR_VERSION, &major_ver));
    glCheck(glGetIntegerv(GL_MINOR_VERSION, &minor_ver));
    glCheck(glGetIntegerv(GL_MAX_ELEMENTS_VERTICES, &elements_vertices));
    glCheck(glGetIntegerv(GL_MAX_ELEMENTS_INDICES, &elemetns_indices));
    glCheck(glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &texture_image_units));
    glCheck(glGetIntegerv(GL_MAX_TEXTURE_SIZE, &tex_size));
    glCheck(glGetIntegerv(GL_MAX_VIEWPORT_DIMS, viewport_dims));
    glCheck(glGetBooleanv(GL_DOUBLEBUFFER, &doublebuffer));

    settings_descriptions2.push_back({ "GL_MAJOR_VERSION", major_ver });
    settings_descriptions2.push_back({ "GL_MINOR_VERSION", minor_ver });
    settings_descriptions2.push_back({ "GL_MAX_ELEMENTS_VERTICES", elements_vertices });
    settings_descriptions2.push_back({ "GL_MAX_ELEMENTS_INDICES", elemetns_indices });
    settings_descriptions2.push_back({ "GL_MAX_TEXTURE_IMAGE_UNITS", texture_image_units });
    settings_descriptions2.push_back({ "GL_MAX_TEXTURE_SIZE", tex_size });

    view_dims = { "GL_MAX_VIEWPORT_DIMS", viewport_dims };
    store[0] = viewport_dims[0];
    store[1] = viewport_dims[1];

    bul = { "GL_DOUBLEBUFFER", doublebuffer };

    std::cout << "GL_VENDOR: " << str_ven << '\n'
            << "GL_RENDERER: " << str_ren << '\n'
        << "GL_VERSION: " << str_ver << '\n'
        << "GL_SHADING_LANGUAGE_VERSION: " << str_sver << '\n'
        << "GL_MAJOR_VERSION: " << major_ver << '\n'
        << "GL_MINOR_VERSION: " << minor_ver << '\n'
        << "GL_MAX_ELEMENTS_VERTICES: " << elements_vertices << '\n'
        << "GL_MAX_ELEMENTS_INDICES: " << elemetns_indices << '\n'
        << "GL_MAX_TEXTURE_IMAGE_UNITS: " << texture_image_units << '\n'
        << "GL_MAX_TEXTURE_SIZE: " << tex_size << '\n'
        << "GL_MAX_VIEWPORT_DIMS: " <<  viewport_dims[0] << " x " << viewport_dims[1] << '\n'
        << "GL_DOUBLEBUFFER: " << (doublebuffer ? "True" : "False") << '\n';

}


