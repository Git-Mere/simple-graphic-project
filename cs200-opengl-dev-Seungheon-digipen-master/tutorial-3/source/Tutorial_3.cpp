/*!************************************************************************
\file Tutorial_3.cpp
\author Seungheon Jeon, Rudy Castan
\par DP email: seungheon.jeon\@digipen.edu
\par Course: CS200/fall
\par CS200 Tutorials 3 - Geometric Instancing Using Model TransformsVirtual programming lab
\date 09-17-2023
\brief
This source file implement Tutorial_3's member functions and several functions.
**************************************************************************/

#include "Tutorial_3.h"

#include "Random.h"
#include "angles.h"
#include "color3.h"
#include "glCheck.h"
#include "vec2.h"

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <algorithm>
#include <imgui.h>
#include <iostream>



util::owner<IProgram*> create_program(int viewport_width, int viewport_height)
{
    return new Tutorial_3(viewport_width, viewport_height);
}

/*!
* \brief construct many thing such as shader, setting object's buffer, setting viewport
* \param int viewport_width set width.
* \param int viewport_height set height.
*/
Tutorial_3::Tutorial_3(int viewport_width, int viewport_height)
{
    glCheck(glClearColor(1, 1, 1, 1));
    glCheck(glViewport(0, 0, viewport_width, viewport_height));

     random_r = util::random(0.0f, 1.f);
     random_g = util::random(0.0f, 1.f);
     random_b = util::random(0.0f, 1.f);

    create_models();

    shader = GLShader("Basic Shader",
        { {GLShader::VERTEX, "assets/shaders/model_to_ndc_pos2d_clr.vert"},
        {GLShader::FRAGMENT, "assets/shaders/basic_vtx_clr_attribute.frag"} });

    objects.reserve(MAX_NUMBER_OBJECTS);
    create_object();

    timing.start_time = glCheck(SDL_GetTicks64());
    timing.prev_time  = timing.start_time;
}

/*!
* \brief update timing and update object's rotate.
*/
void Tutorial_3::Update()
{
    update_timing();
    for (auto& a : objects) {
        a.rotation += util::to_radians(a.rotation_speed) * timing.deltatime;
    }
}

namespace
{
    std::span<const float, 3 * 3> to_span(const mat3& m)
    {
        return std::span<const float, 3 * 3>(&m.elements[0][0], 9);
    }
}

/*!
* \brief make each matrix of object and send this to shader. finally draw to screen.
*/
void Tutorial_3::Draw()
{
    glCheck(glClear(GL_COLOR_BUFFER_BIT));

    if (poly_mode.type == PolyMode::Point) {
        glCheck(glPolygonMode(GL_FRONT_AND_BACK, GL_POINT));
        glCheck(glPointSize(5.0f));
    }
    else if (poly_mode.type == PolyMode::Line) {
        glCheck(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
        glCheck(glLineWidth(2.0f));
    }
    else if (poly_mode.type == PolyMode::Fill) {
        glCheck(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
    }

    glCheck(shader.Use());
    for (const auto& object : objects)
    {
        const mat3 transmatrix = mat3::build_translation(object.position);
        const mat3 Hscalematrix = mat3::build_scale(1/ WORLD_SIZE_MAX, 1 / WORLD_SIZE_MAX);
        const mat3 scalematrix = mat3::build_scale(object.scale);
        const mat3 rotatematrix = mat3{cos(object.rotation), sin(object.rotation),0 ,-sin(object.rotation) ,cos(object.rotation),0,0,0,1 };
        mat3 model_to_world =  transmatrix * rotatematrix * scalematrix;

        const mat3 model_to_ndc = Hscalematrix * model_to_world;
        glCheck(shader.SendUniform("uModelToNDC", to_span(model_to_ndc)));
        glCheck(models[object.which_model].Use());

        glCheck(GLDrawVertices(models[object.which_model]));
    }

    glCheck(shader.Use(false));
    glCheck(models[Rectangle].Use(false));
    glCheck(models[MysteryShape].Use(false));
    glCheck(glLineWidth(1.0f));
    glCheck(glPointSize(1.0f));
}

void Tutorial_3::ImGuiDraw()
{
    glCheck(ImGui::Begin("Program Info"));
    {
        glCheck(ImGui::LabelText("FPS", "%.1f", timing.fps));

        if (objects.size() == MAX_NUMBER_OBJECTS)
        {
            glCheck(ImGui::BeginDisabled());
            glCheck(ImGui::ArrowButton("+", ImGuiDir_Up));
            glCheck(ImGui::EndDisabled());
        }
        else
        {
            if (const bool increase = ImGui::ArrowButton("+", ImGuiDir_Up); increase)
            {
                increase_number_of_objects();
            }
        }
        glCheck(ImGui::SameLine());
        if (objects.size() == 1)
        {
            glCheck(ImGui::BeginDisabled());
            glCheck(ImGui::ArrowButton("-", ImGuiDir_Down));
            glCheck(ImGui::EndDisabled());
        }
        else
        {
            if (const bool decrease = ImGui::ArrowButton("-", ImGuiDir_Down); decrease)
            {
                decrease_number_of_objects();
            }
        }
        glCheck(ImGui::SameLine());
        glCheck(ImGui::Text("%s %lu", "Size", static_cast<unsigned long>(objects.size())));
        glCheck(ImGui::LabelText("# Boxes", "%d", number_of_boxes));
        glCheck(ImGui::LabelText("# Mystery Shapes", "%d", number_of_mystery));


        constexpr const char* items[]             = {"GL_POINT", "GL_LINE", "GL_FILL"};
        constexpr PolyMode    types[]             = {PolyMode::Point, PolyMode::Line, PolyMode::Fill};
        const char* const     combo_preview_value = items[poly_mode.item_current_idx];
        if (ImGui::BeginCombo("PolygonMode", combo_preview_value, 0))
        {
            for (int n = 0; n < IM_ARRAYSIZE(items); n++)
            {
                const bool is_selected = (poly_mode.item_current_idx == n);
                if (ImGui::Selectable(items[n], is_selected))
                {
                    poly_mode.item_current_idx = n;
                    poly_mode.type             = types[n];
                }
                if (is_selected) // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                    glCheck(ImGui::SetItemDefaultFocus());
            }
            glCheck(ImGui::EndCombo());
        }
    }
    glCheck(ImGui::End());
}

void Tutorial_3::HandleEvent(SDL_Window& sdl_window, const SDL_Event& event)
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

void Tutorial_3::on_window_resized(SDL_Window& sdl_window) noexcept
{
    int width = 0, height = 0;
    glCheck(SDL_GL_GetDrawableSize(&sdl_window, &width, &height));
    glCheck(glViewport(0, 0, width, height));
}

/*!
* \brief update timing including delta time
*/
void Tutorial_3::update_timing() noexcept
{
    const auto current_time = glCheck(SDL_GetTicks64());

    timing.deltatime = current_time - timing.prev_time;

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

/*!
* \brief create models's information such as position, color.
*/
void Tutorial_3::create_models()
{
    { 
        std::vector<vec2>   pos_vtx{ vec2{ -0.5f, -0.5f }, vec2{ 0.5f, -0.5f }, vec2{ 0.5f, 0.5f },vec2{ 0.5f, 0.5f },vec2{ -0.5f, 0.5f },vec2{ -0.5f, -0.5f } };
        std::vector<color3> clr_vtx{ color3{ random_r, random_r, random_b }, color3{ random_r, random_b, random_r }, 
            color3{ random_b, random_g, random_r }, color3{ random_b, random_b, random_g }, 
            color3{ random_g, random_g, random_b }, color3{  random_b, random_g, random_b} };

        GLsizei       positions_byte_size = static_cast<GLsizei>(sizeof(vec2) * pos_vtx.size());
        GLsizei       colors_byte_size = static_cast<GLsizei>(sizeof(color3) * clr_vtx.size());

        GLsizei       buffer_size = positions_byte_size + colors_byte_size;
        GLVertexBuffer       buffer(buffer_size);
        glCheck(buffer.SetData(std::span(pos_vtx)));
        glCheck(buffer.SetData(std::span(clr_vtx), positions_byte_size));

        glCheck(models[Rectangle].SetVertexCount(static_cast<int>(pos_vtx.size())));
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

        glCheck(models[Rectangle].AddVertexBuffer(std::move(buffer), { position, color }));
        glCheck(models[Rectangle].SetPrimitivePattern(GLPrimitive::Triangles));

    }

    {
        float height = 0.2f;
        float sht = 0.3f;
        float lng = 0.5f;
        std::vector<vec2>   pos_vtx{ vec2{ -lng, height }, vec2{ -sht, height }, vec2{ sht, -height },
                                vec2{ -sht, height }, vec2{ lng, -height }, vec2{ sht, -height },
                                vec2{ -lng, -height }, vec2{ sht, height }, vec2{ lng, height },
                                vec2{ lng, height }, vec2{ -sht, -height }, vec2{ -lng, -height } };
        std::vector<color3> clr_vtx{ color3{ 0.25, 0.25, 0.25 }, color3{ 0.25, 0.25, 0.25 }, color3{ 0.25, 0.25, 0.25 },
                                    color3{ 1, 0, 0 }, color3{ 0.25, 0.25, 0.25 }, color3{ 0.25, 0.25, 0.25 },
                                    color3{ 0.25, 0.25, 0.25 }, color3{ 0, 1, 0 }, color3{ 0.25, 0.25, 0.25 },
                                    color3{ 0.25, 0.25, 0.25 }, color3{ 0.25, 0.25, 0.25 }, color3{ 0, 0, 1 } }; // new for tristrip

        GLsizei       positions_byte_size = static_cast<GLsizei>(sizeof(vec2) * pos_vtx.size());
        GLsizei       colors_byte_size = static_cast<GLsizei>(sizeof(color3) * clr_vtx.size());

        GLsizei       buffer_size = positions_byte_size + colors_byte_size;
        GLVertexBuffer       buffer(buffer_size);
        glCheck(buffer.SetData(std::span(pos_vtx)));
        glCheck(buffer.SetData(std::span(clr_vtx), positions_byte_size));

        glCheck(models[MysteryShape].SetVertexCount(static_cast<int>(pos_vtx.size())));
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

        glCheck(models[MysteryShape].AddVertexBuffer(std::move(buffer), { position, color }));
        glCheck(models[MysteryShape].SetPrimitivePattern(GLPrimitive::Triangles));
    }
}

/*!
* \brief object 2 times according to the total number.
*/
void Tutorial_3::increase_number_of_objects()
{
    int total = number_of_boxes + number_of_mystery;
    for (int i = 0; i < total; i++) {
        create_object();
    }

}

/*!
* \brief object 1/2 times according to the total number.
*/
void Tutorial_3::decrease_number_of_objects()
{
    int total = number_of_boxes + number_of_mystery;
    for (int i = 0; i < total / 2; i++) {
        if (objects[0].which_model == ModelType::Rectangle) { number_of_boxes--; }
        else if (objects[0].which_model == ModelType::MysteryShape) { number_of_mystery--; }
        objects.erase(objects.begin());
    }

}

/*!
* \brief create object that have random values.
*/
void Tutorial_3::create_object()
{
    Object tmp;

    tmp.which_model = static_cast<ModelType>(util::random(0, 2));
    tmp.position = vec2{ util::random(WORLD_SIZE_MIN, WORLD_SIZE_MAX) ,util::random(WORLD_SIZE_MIN, WORLD_SIZE_MAX) };
    tmp.scale = vec2{ util::random(OBJ_SIZE_MIN, OBJ_SIZE_MAX) , util::random(OBJ_SIZE_MIN, OBJ_SIZE_MAX) };
    tmp.rotation = util::random(0.0, util::to_radians(360.f));
    tmp.rotation_speed = util::random(ROT_SPEED_MIN, ROT_SPEED_MAX);
    
    if (tmp.which_model == ModelType::Rectangle) { number_of_boxes++; }
    else if (tmp.which_model == ModelType::MysteryShape) { number_of_mystery++; }
    objects.push_back(tmp);
}

