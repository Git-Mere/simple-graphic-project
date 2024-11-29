/*!************************************************************************
\file Tutorial_4.cpp
\author Seungheon Jeon, Rudy Castan
\par DP email: seungheon.jeon\@digipen.edu
\par Course: CS200/fall
\par CS200 Tutorial 4 - Implementing 2D Interactive CameraVirtual programming lab
\date 09-29-2023
\brief
This source file implement Tutorial_4's member functions
**************************************************************************/

#include "Tutorial_4.h"

#include "angles.h"
#include "color3.h"
#include "glCheck.h"
#include "vec2.h"

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <algorithm>
#include <array>
#include <fstream>
#include <imgui.h>
#include <iostream>
#include <sstream>


util::owner<IProgram *> create_program(int viewport_width, int viewport_height)
{
    return new Tutorial_4(viewport_width, viewport_height);
}

Tutorial_4::Tutorial_4(int viewport_width, int viewport_height)
{
    glCheck(glViewport(0, 0, viewport_width, viewport_height));

    caminfo.camera_view.SetFramebufferSize(viewport_width, viewport_height);

    glCheck(glClearColor(1, 1, 1, 1));

    load_scene("assets/scenes/tutorial-4.scn");

    timing.start_time = glCheck(SDL_GetTicks64());
    timing.prev_time = timing.start_time;
}

void Tutorial_4::Update()
{
    update_timing();

    // Update Camera :
    caminfo.camera.MoveUp(caminfo.move_speed * timing.DeltaTime * caminfo.move_scalar);
    caminfo.camera.MoveRight(caminfo.move_speed * timing.DeltaTime * caminfo.strafe_scalar);
    caminfo.camera.Rotate(caminfo.obj.angle_speed * timing.DeltaTime * caminfo.turn_scalar);

    const mat3 world_to_cam = caminfo.camera.BuildWorldToCamera();
    const mat3 cam_to_ndc = caminfo.camera_view.BuildCameraToNDC();
    const mat3 world_to_ndc = cam_to_ndc * world_to_cam;

    // for each game object
    for (auto& object : objects) {
        object.angle_disp += object.angle_speed * timing.DeltaTime;
        const mat3 T = mat3::build_translation(object.position);
        const mat3 R = mat3{ cos(object.angle_disp), sin(object.angle_disp),0 ,-sin(object.angle_disp) ,cos(object.angle_disp),0,0,0,1 };
        const mat3 H = mat3::build_scale(object.scaling);
        mat3 model_to_world = T * R * H;
        object.model_to_ndc = world_to_ndc * model_to_world;
    }

    const mat3 T = mat3::build_translation(caminfo.camera.Position);
    float radian_value = caminfo.camera.GetOrientation();
    const mat3 R = mat3{ cos(radian_value), sin(radian_value),0 ,-sin(radian_value) ,cos(radian_value),0,0,0,1 };
    const mat3 H = mat3::build_scale(caminfo.obj.scaling);
    caminfo.obj.model_to_ndc = world_to_ndc * T * R * H;

    compute_mouse_coordinates();
}

namespace
{
    std::span<const float, 3 * 3> to_span(const mat3 &m)
    {
        return std::span<const float, 3 * 3>(&m.elements[0][0], 9);
    }
    std::span<const float, 3> to_span(const color3 &c)
    {
        return std::span<const float, 3>(&c.elements[0], 3);
    }
}

void Tutorial_4::Draw()
{
    // Clear the color buffer
    glCheck(glClear(GL_COLOR_BUFFER_BIT));

    if (!objects.empty())
    {
        unsigned current_shader = 0;
        unsigned current_model = 0;
        shaders.at(0).Use();
        models.at(0).Use();
        for (const auto& object : objects) {
            if (object.shader_index != current_shader) {
                current_shader = object.shader_index;
                shaders.at(current_shader).Use();
            }
            if (object.model_index != current_model) {
                current_model = object.model_index;
                models.at(current_model).Use();
            }

            glCheck(shaders.at(current_shader).SendUniform("uModelToNDC", to_span(object.model_to_ndc)));
            glCheck(shaders.at(current_shader).SendUniform("uFillColor", to_span(object.color)));

            glCheck(GLDrawIndexed(models[current_model]));
        }
        glCheck(shaders.at(caminfo.obj.shader_index).Use());
        glCheck(models.at(caminfo.obj.model_index).Use());

        glCheck(shaders.at(caminfo.obj.shader_index).SendUniform("uModelToNDC", to_span(caminfo.obj.model_to_ndc)));
        glCheck(shaders.at(caminfo.obj.shader_index).SendUniform("uFillColor", to_span(caminfo.obj.color)));

        glCheck(GLDrawIndexed(models.at(caminfo.obj.model_index)));
    }
}

void Tutorial_4::ImGuiDraw()
{
    ImGui::Begin("Program Info");
    {
        ImGui::LabelText("FPS", "%.1f", static_cast<double>(timing.fps));
        ImGui::LabelText("Delta time", "%.3f seconds", static_cast<double>(timing.DeltaTime));
        ImGui::LabelText("Mouse Positions", "Device(%4.0f,%4.0f)\nCamera(%4.0f,%4.0f)\nWorld (%4.0f,%4.0f)", static_cast<double>(environment.mouseX),
                         static_cast<double>(environment.mouseY),                                                             // device space
                         static_cast<double>(environment.mouseCamSpaceX), static_cast<double>(environment.mouseCamSpaceY),    // in Camera Space
                         static_cast<double>(environment.mouseWorldSpaceX), static_cast<double>(environment.mouseWorldSpaceY) // in World Space
        );
        const auto cam_pos = caminfo.camera.Position;
        ImGui::LabelText("Camera World Position", "(%.1f,%.1f)", static_cast<double>(cam_pos.x), static_cast<double>(cam_pos.y));
        const auto cam_orientation = int(util::to_degrees(caminfo.camera.GetOrientation()) + 360) % 360;
        ImGui::LabelText("Camera World Orientation", "%d degrees", cam_orientation);
        const auto cam_size = caminfo.camera_view.CalcViewSizeWithZoom();
        ImGui::LabelText("Camera View Size", "%.1f x %.1f", static_cast<double>(cam_size.width), static_cast<double>(cam_size.height));
        auto zoom = caminfo.camera_view.GetZoom() * 100.0f;
        if (ImGui::SliderFloat("Zoom", &zoom, 25.0f, 400.0f, "%.0f%%"))
        {
            caminfo.camera_view.SetZoom(zoom / 100.0f);
        }

        ImGui::Checkbox("First Person Camera", &caminfo.camera.IsFirstPerson);

        if (ImGui::Button("See Camera Controls?"))
            ImGui::OpenPopup("Camera Controls");
        const ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        if (ImGui::BeginPopup("Camera Controls", ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("Left/Right Arrows to turn camera left & right");
            ImGui::Separator();
            ImGui::Text("Up/Down Arrows to turn move camera forward & backward");
            ImGui::Separator();
            ImGui::Text("A/D keys to strafe left & right");
            ImGui::Separator();
            ImGui::SetItemDefaultFocus();
            if (ImGui::Button("Okay"))
            {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
    }
    ImGui::End();
}

void Tutorial_4::HandleEvent(SDL_Window &sdl_window, const SDL_Event &event)
{
    switch (event.type)
    {
    case SDL_MOUSEMOTION:
        // make sure we only save mouse positions for the main window and not some ImGui window
        if (event.motion.windowID == SDL_GetWindowID(&sdl_window))
        {
            environment.mouseX = event.motion.x;
            environment.mouseY = event.motion.y;
        }
        break;
    case SDL_KEYDOWN:
    case SDL_KEYUP:
    {
        float pressed = 1.0f;
        //float release = 0.0f;
        if (event.key.state == SDL_PRESSED)
        {
            // pressed -> float of 1
            pressed = 1.0f;
        }
        else
        { // SDL_RELEASED
          // release -> float of 0
            pressed = 0.0f;
        }
        switch (event.key.keysym.sym)
        {
        case SDLK_UP:
            // caminfo.move_scalar <- pressed state
            caminfo.move_scalar = pressed;
            break;
        case SDLK_DOWN:
            // caminfo.move_scalar <- negative pressed state
            caminfo.move_scalar = -pressed;
            break;
        case SDLK_LEFT:
            // caminfo.turn_scalar <- pressed state
            caminfo.turn_scalar = pressed;
            break;
        case SDLK_RIGHT:
            // caminfo.turn_scalar <- negative pressed state
            caminfo.turn_scalar = -pressed;
            break;
        case SDLK_a:
            // caminfo.strafe_scalar <- negative pressed state
            caminfo.strafe_scalar = -pressed;
            break;
        case SDLK_d:
            // caminfo.strafe_scalar <- pressed state
            caminfo.strafe_scalar = pressed;
            break;
        }
    }
    break;
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

void Tutorial_4::on_window_resized(SDL_Window &sdl_window) noexcept
{
    int width = 0, height = 0;
    glCheck(SDL_GL_GetDrawableSize(&sdl_window, &width, &height));
    glCheck(glViewport(0, 0, width, height));
    caminfo.camera_view.SetFramebufferSize(width, height);
    
}

void Tutorial_4::update_timing() noexcept
{
    const auto current_time = glCheck(SDL_GetTicks64());
    const auto delta_ms = current_time - timing.prev_time;
    timing.DeltaTime = delta_ms / 1'000.0f;

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

void Tutorial_4::compute_mouse_coordinates() noexcept
{
    
    const mat3 BWDTC = caminfo.camera_view.BuildWindowDeviceToCamera() * 
        mat3::build_translation(static_cast<float>(environment.mouseX), static_cast<float>(environment.mouseY));
    environment.mouseCamSpaceX = BWDTC.elements[2][0];
    environment.mouseCamSpaceY = BWDTC.elements[2][1];

    const mat3 BCTW = caminfo.camera.BuildCameraToWorld() * 
        mat3::build_translation(static_cast<float>(environment.mouseCamSpaceX), static_cast<float>(environment.mouseCamSpaceY));
    environment.mouseWorldSpaceX = BCTW.elements[2][0];
    environment.mouseWorldSpaceY = BCTW.elements[2][1];
}

void Tutorial_4::load_scene(const std::filesystem::path& scene_file_path)
{
    std::ifstream sceneFile{ scene_file_path };
    if (!sceneFile)
    {
        throw std::runtime_error(std::string("ERROR: Unable to open scene file: ") + scene_file_path.string());
    }

    std::string line;
    std::getline(sceneFile, line);

    int objectCount;
    std::istringstream{ line } >> objectCount;
    objects.reserve(static_cast<unsigned>(objectCount));

    std::string           modelName;
    std::string           objectName;
    std::string           shaderProgramName;
    std::filesystem::path vertexShaderPath, fragmentShaderPath;
    float                 rotationAngular, rotationSpeedAngular;

    for (int i = 0; i < objectCount; ++i)
    {
        Object object;
        std::getline(sceneFile, line);
        std::istringstream{ line } >> modelName;

        std::getline(sceneFile, line);
        std::istringstream{ line } >> object.name;

        object.model_index = load_mesh_and_get_id(std::filesystem::path("assets") / "meshes" / (modelName + ".msh"));

        std::getline(sceneFile, line);
        std::istringstream{ line } >> shaderProgramName >> vertexShaderPath >> fragmentShaderPath;
        object.shader_index = load_shader_and_get_id(shaderProgramName, vertexShaderPath, fragmentShaderPath);

            // RGB Color
        std::getline(sceneFile, line);
        std::istringstream{ line } >> object.color.red >> object.color.green >> object.color.blue;
            // Width Height
        std::getline(sceneFile, line);
        std::istringstream{ line } >> object.scaling.x >> object.scaling.y;
            // Orientation, Rotation Speed
        std::getline(sceneFile, line);
        std::istringstream{ line } >> rotationAngular >> rotationSpeedAngular;
        object.angle_disp = util::to_radians(rotationAngular);
        object.angle_speed = util::to_radians(rotationSpeedAngular);
            // 2D Position
        std::getline(sceneFile, line);
        std::istringstream{ line } >> object.position.x >> object.position.y;


        if (object.name != "Camera")
        {
            objects.emplace_back(std::move(object));
        }
        else
        {
            caminfo.obj = object;
            caminfo.camera.SetOrientation(object.angle_disp);
            caminfo.camera.Position = object.position;
        }
    }
}

unsigned Tutorial_4::load_mesh_and_get_id(const std::filesystem::path& mesh_file_path)
{
    auto [iter, is_new] = asset_to_index.insert({ mesh_file_path.string(), 0 });
    auto& location = iter->second;
    if (!is_new)
    {
        return location;
    }

    std::ifstream meshFile{ mesh_file_path };
    if (!meshFile)
    {
        throw std::runtime_error{ std::string("ERROR: Unable to open mesh file: ") + mesh_file_path.string() };
    }

    std::vector<vec2>           vertex_positions;
    std::vector<unsigned short> vertex_indices;

    std::string       modelName;
    float             x = 0, y = 0;
    GLushort          index = 0;
    std::string       line;
    char              mode = 0;
    GLPrimitive::Type primitive_type = GLPrimitive::Points;
    bool three = true;

    while (meshFile)
    {
        std::getline(meshFile, line);
        if (line.empty())
        {
            continue;
        }

        std::istringstream sstr{ line };
        sstr >> mode;
        switch (mode) {
        case 'n':
            sstr >> modelName;
            break;
        case 'v':
            sstr >> x >> y;
            vertex_positions.emplace_back(x, y);
            break;
        case 't':
            primitive_type = GLPrimitive::Triangles;
            sstr >> index;
            vertex_indices.push_back(index);
            sstr >> index;
            vertex_indices.push_back(index);
            sstr >> index;
            vertex_indices.push_back(index);
            break;
        case 'f':
            primitive_type = GLPrimitive::TriangleFan;
            if (three) {
                sstr >> index;
                vertex_indices.push_back(index);
                sstr >> index;
                vertex_indices.push_back(index);
                sstr >> index;
                vertex_indices.push_back(index);
                three = false;
            }
            else {
                sstr >> index;
                vertex_indices.push_back(index);
            }
            break;

        }
    }

    GLVertexArray model(primitive_type);

    GLVertexBuffer       buffer(static_cast<GLsizei>(sizeof(vec2) * vertex_positions.size()));
    glCheck(buffer.SetData(std::span(vertex_positions)));

    glCheck(model.SetVertexCount(static_cast<int>(vertex_positions.size())));

    GLAttributeLayout position;
    position.component_type = GLAttributeLayout::Float;
    position.component_dimension = GLAttributeLayout::_2;
    position.normalized = false;
    position.vertex_layout_location = 0; // 1st field is 0 index based
    position.stride = sizeof(vec2);
    position.offset = 0;
    position.relative_offset = 0;

    GLIndexBuffer       index_buffer(vertex_indices);

    glCheck(model.AddVertexBuffer(std::move(buffer), { position }));
    glCheck(model.SetIndexBuffer(std::move(index_buffer)));

    location = static_cast<unsigned>(models.size());
    models.emplace_back(std::move(model));

    return location;
}

unsigned Tutorial_4::load_shader_and_get_id(std::string_view name, const std::filesystem::path& vertex_filepath, const std::filesystem::path& fragment_filepath)
{
    // Insert if 1st time otherwise get what is there
    auto [iter, is_new] = asset_to_index.insert({ std::string(name), 0 });
    auto& location = iter->second;
    if (!is_new)
    {
        return location;
    }
    location = static_cast<unsigned>(shaders.size());
    shaders.emplace_back(GLShader(name, { {GLShader::VERTEX, vertex_filepath}, {GLShader::FRAGMENT, fragment_filepath} }));
    return location;
}