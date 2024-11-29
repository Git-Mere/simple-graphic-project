/*!************************************************************************
\file Tutorial_2.cpp
\author Seungheon Jeon, Rudy Castan
\par DP email: seungheon.jeon\@digipen.edu
\par Course: CS200/fall
\par CS200 Tutorial 2 - Viewports and OpenGL Primitives
\date 09-13-2023
\brief
This source file implement Tutorial_2's member functions and several functions.
**************************************************************************/

#include "Tutorial_2.h"

#include "Random.h"
#include "angles.h"
#include "color3.h"
#include "glCheck.h"
#include "vec2.h"

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <cmath>
#include <array>
#include <imgui.h>
#include <iostream>
enum ViewportPosition {
    TopLeft = 0,
    TopRight = 1,
    BottomLeft = 2,
    BottomRight = 3
};

struct
{
    int x = 0, y = 0, width = 0, height = 0;
} viewports[4];

util::owner<IProgram*> create_program(int viewport_width, int viewport_height)
{
    return new Tutorial_2(viewport_width, viewport_height);
}

Tutorial_2::Tutorial_2(int viewport_width, int viewport_height)
{
    glCheck(glClearColor(1, 1, 1, 1));

    window_width = viewport_width;
    window_height = viewport_height;

    set_viewports(viewport_width, viewport_height);
    shader = GLShader("Basic Shader",
        { {GLShader::VERTEX, "assets/shaders/pass_thru_pos2d_clr.vert"},
        {GLShader::FRAGMENT, "assets/shaders/basic_vtx_clr_attribute.frag"} });

    create_points_model();
    create_lines_model();
    create_trifan_model();
    create_tristrip_model();

    timing.start_time = glCheck(SDL_GetTicks64());
    timing.prev_time = timing.start_time;
}

void Tutorial_2::Update()
{
    update_timing();
}


void Tutorial_2::Draw()
{
    glCheck(glClear(GL_COLOR_BUFFER_BIT));

    shader.Use();

    glCheck(glViewport(viewports[TopLeft].x, viewports[TopLeft].y, viewports[TopLeft].width, viewports[TopLeft].height));
    {
        glCheck(glPointSize(10.0f));
        models[TopLeft].Use();
        constexpr int layout_location = 1;
        glCheck(glVertexAttrib3f(layout_location, 1.f, 0.0f, 0.f)); // red for color attribute for points
        glCheck(GLDrawVertices(models[TopLeft]));
        glCheck(glPointSize(1.0f));
    }
    glCheck(glViewport(viewports[TopRight].x, viewports[TopRight].y, viewports[TopRight].width, viewports[TopRight].height));
    {
        glCheck(glLineWidth(3.0f));
        models[TopRight].Use();
        constexpr int layout_location = 1;
        glCheck(glVertexAttrib3f(layout_location, 0.f, 0.0f, 1.f)); // blue for color attribute for lines        
        glCheck(GLDrawVertices(models[TopRight]));
        glCheck(glLineWidth(1.0f));
    }
    glCheck(glViewport(viewports[BottomLeft].x, viewports[BottomLeft].y, viewports[BottomLeft].width, viewports[BottomLeft].height));
    {
        models[BottomLeft].Use();
        glCheck(GLDrawVertices(models[BottomLeft]));
    }
    glCheck(glViewport(viewports[BottomRight].x, viewports[BottomRight].y, viewports[BottomRight].width, viewports[BottomRight].height));
    {
        models[BottomRight].Use();
        glCheck(GLDrawIndexed(models[BottomRight]));
    }

    shader.Use(false);
}

void Tutorial_2::ImGuiDraw()
{
    ImGui::Begin("Program Info");
    {
        ImGui::LabelText("FPS", "%.1f", timing.fps);
        ImGui::LabelText("GL_POINTS", "# Points    %4d", models[TopLeft].GetVertexCount());
        ImGui::LabelText("GL_LINES", "# Lines     %4d # Verts   %4d", models[TopRight].GetVertexCount() / 2, models[TopRight].GetVertexCount());
        ImGui::LabelText("GL_TRIANGLE_FAN", "# Triangles %4d # Verts   %4d", models[BottomLeft].GetVertexCount() - 2, models[BottomLeft].GetVertexCount());
        ImGui::LabelText("GL_TRIANGLE_STRIP", "# Triangles %4d # Indices %4d", models[3].GetIndicesCount() - 2, models[3].GetIndicesCount());
    }
}

void Tutorial_2::HandleEvent(SDL_Window& sdl_window, const SDL_Event& event)
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

void Tutorial_2::on_window_resized(SDL_Window& sdl_window) noexcept
{
    int width = 0, height = 0;
    SDL_GL_GetDrawableSize(&sdl_window, &width, &height);
    set_viewports(width, height);
}

void Tutorial_2::update_timing() noexcept
{
    const auto current_time = SDL_GetTicks64();

    timing.prev_time = current_time;
    const auto elapsed_ms = current_time - timing.start_time;

    timing.count++;
    if (elapsed_ms > 1'000)
    {
        timing.fps = timing.count / (elapsed_ms / 1'000.0f);
        timing.start_time = current_time;
        timing.count = 0;
    }
}



void Tutorial_2::set_viewports(int width, int height)
{
    viewports[TopLeft] = { 0,height / 2,width / 2,height / 2 };
    viewports[TopRight] = { width / 2,height / 2,width / 2,height / 2 };
    viewports[BottomLeft] = { 0,0,width / 2,height / 2 };
    viewports[BottomRight] = { width / 2, 0, width / 2, height / 2 };
}

void Tutorial_2::create_points_model()
{
    constexpr int     stacks = 20;
    constexpr int     slices = 20;
    constexpr int     count{ (stacks + 1) * (slices + 1) };

    std::vector<vec2> pos_vtx(count);
    float x = 2.0f / stacks;
    float y = 2.0f / slices;

    for (int i = 0; i < stacks + 1; i++) {
        for (int j = 0; j < slices + 1; j++) {
            pos_vtx[(i * (stacks + 1)) + j].x = -1 + (i * x);
            pos_vtx[(i * (stacks + 1)) + j].y = -1 + (j * y);
        }
    }

    // Make sure to properly set the vertex count for drawing later
    glCheck(models[TopLeft].SetVertexCount(static_cast<int>(pos_vtx.size())));

    // Describe the layout of our array of 2d positions
    GLAttributeLayout position;
    position.component_dimension = GLAttributeLayout::_2;
    position.component_type = GLAttributeLayout::Float;
    position.normalized = false;
    position.offset = 0;
    position.relative_offset = 0;
    position.stride = sizeof(vec2);
    position.vertex_layout_location = 0;

    // Provide the vertices as a GLVertexBuffer and the layout of it 
    glCheck(models[TopLeft].AddVertexBuffer(GLVertexBuffer(std::span{ pos_vtx }), { position }));
    // Make sure we set the primitive pattern to points since we want to draw points
    glCheck(models[TopLeft].SetPrimitivePattern(GLPrimitive::Points));
}

void Tutorial_2::create_lines_model()
{
    constexpr int slices = 40;
    constexpr int count{ (slices + 1) * 2 }; // 2 means one line needs two points
    std::vector<vec2> pos_vtx(count * 2);
    unsigned          index{ 0 };
    constexpr float   u{ 2.f / static_cast<float>(slices) };
    for (int ver{ 0 }; ver <= slices; ++ver)
    {
        const float y{ u * static_cast<float>(ver) - 1.f };
        pos_vtx[index++] = vec2{ -1.0f, y };
        pos_vtx[index++] = vec2{ 1.0f, y };
    }
    for (int col{ 0 }; col <= slices; ++col)
    {
        const float x{ u * static_cast<float>(col) - 1.f };
        pos_vtx[index++] = vec2{ x, -1.0f };
        pos_vtx[index++] = vec2{ x, 1.0f };
    }


    glCheck(models[TopRight].SetVertexCount(static_cast<int>(pos_vtx.size())));

    // Describe the layout of our array of 2d positions
    GLAttributeLayout position;
    position.component_dimension = GLAttributeLayout::_2;
    position.component_type = GLAttributeLayout::Float;
    position.normalized = false;
    position.offset = 0;
    position.relative_offset = 0;
    position.stride = sizeof(vec2);
    position.vertex_layout_location = 0;

    // Provide the vertices as a GLVertexBuffer and the layout of it 
    glCheck(models[TopRight].AddVertexBuffer(GLVertexBuffer(std::span{ pos_vtx }), { position }));
    // Make sure we set the primitive pattern to points since we want to draw points
    glCheck(models[TopRight].SetPrimitivePattern(GLPrimitive::Lines));
}

void Tutorial_2::create_trifan_model()
{
    constexpr int slices = 30;

    std::vector<vec2> positions(slices + 2);
    float ceta = 360.0f / slices;
    positions[0] = vec2{ 0,0 };
    for (int i = 1; i < slices + 2; i++) {
        float angle = util::to_radians(ceta * i);
        positions[i] = { static_cast<float>(cos(angle)) , static_cast<float>(sin(angle)) };
    }
    positions[31] = positions[1];

    GLsizei       positions_byte_size = static_cast<GLsizei>(sizeof(vec2) * positions.size());

    std::vector<vec3> colors(slices + 2);
    for (int i = 0; i < slices + 1; i++) {
        colors[i] = { util::random(0.0, 1.0f), util::random(0.0, 1.0f) , util::random(0.0, 1.0f) };
    }
    colors[31] = colors[1];
    GLsizei       colors_byte_size = static_cast<GLsizei>(sizeof(color3) * colors.size());

    GLsizei       buffer_size = positions_byte_size + colors_byte_size;
    GLVertexBuffer       buffer(buffer_size);
    glCheck(buffer.SetData(std::span(positions)));
    glCheck(buffer.SetData(std::span(colors), positions_byte_size));

    glCheck(models[BottomLeft].SetVertexCount(static_cast<int>(positions.size())));
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

    glCheck(models[BottomLeft].AddVertexBuffer(std::move(buffer), { position, color }));
    glCheck(models[BottomLeft].SetPrimitivePattern(GLPrimitive::TriangleFan));
}

void Tutorial_2::create_tristrip_model()
{
    constexpr short stacks = 15;
    constexpr short slices = 10;

    constexpr unsigned  count{ (stacks + 1) * (slices + 1) };
    std::vector<vec2>   pos_vtx(count);
    std::vector<color3> clr_vtx(count); // new for tristrip
    std::vector<unsigned> indices{};

    constexpr float u{ 2.0f / static_cast<float>(slices) };
    constexpr float v{ 2.0f / static_cast<float>(stacks) };

    for (unsigned row{ 0 }, index{ 0 }; row <= stacks; ++row)
    {
        for (unsigned column{ 0 }; column <= slices; ++column)
        {
            pos_vtx[index] = vec2{ u * static_cast<float>(column) - 1.f, v * static_cast<float>(row) - 1.f };
            clr_vtx[index++] = color3{ util::random(), util::random(), util::random() };
        }
    }
    int help = slices + 1;

    for (int i = 0; i < stacks; i++) {
        std::vector<int> a{ 0 + (help * i),1 + (help * i),2 + (help * i),3 + (help * i),4 + (help * i),
            5 + (help * i),6 + (help * i),7 + (help * i),8 + (help * i),9 + (help * i),10 + (help * i) };
        std::vector<int> b{ 11 + (help * i),12 + (help * i),13 + (help * i),14 + (help * i),15 + (help * i),
            16 + (help * i),17 + (help * i),18 + (help * i),19 + (help * i),20 + (help * i),21 + (help * i) };
        if (i != 0) { indices.push_back(b[0]); }

        for (int j = 0; j < (slices * 2) + 1; j += 2) {
            indices.push_back(b[j / 2]);
            indices.push_back(a[j / 2]);
        }
        if (i + 1 != stacks) {
            indices.push_back(indices[indices.size() - 1]);
        }
    }

    GLsizei       positions_byte_size = static_cast<GLsizei>(sizeof(vec2) * pos_vtx.size());
    GLsizei       colors_byte_size = static_cast<GLsizei>(sizeof(color3) * clr_vtx.size());

    GLsizei       buffer_size = positions_byte_size + colors_byte_size;
    GLVertexBuffer       buffer(buffer_size);
    glCheck(buffer.SetData(std::span(pos_vtx)));
    glCheck(buffer.SetData(std::span(clr_vtx), positions_byte_size));

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

    glCheck(models[BottomRight].AddVertexBuffer(std::move(buffer), { position, color }));
    glCheck(models[BottomRight].SetPrimitivePattern(GLPrimitive::TriangleStrip));

    GLIndexBuffer       index_buffer(indices);
    glCheck(models[BottomRight].SetIndexBuffer(std::move(index_buffer)));
}
