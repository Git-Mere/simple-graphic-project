/*!************************************************************************
\file Tutorial_4.h
\author Seungheon Jeon, Rudy Castan
\par DP email: seungheon.jeon\@digipen.edu
\par Course: CS200/fall
\par CS200 Tutorial 4 - Implementing 2D Interactive CameraVirtual programming lab
\date 09-29-2023
\brief
This header file declare Tutorial_4's member functions and make some struct
**************************************************************************/

#pragma once

#include "Camera.h"
#include "CameraView.h"
#include "GLShader.h"
#include "GLVertexArray.h"
#include "IProgram.h"
#include "angles.h"
#include "color3.h"
#include "mat3.h"

#include <unordered_map>
#include <vector>

class Tutorial_4 : public IProgram
{
public:
    /*!
* \brief construct many thing such as load scene, Set Frame bufferSize, set view port
* \param int viewport_width set width.
* \param int viewport_height set height.
*/
    Tutorial_4(int viewport_width, int viewport_height);

    /*!
* \brief Mainly to compute each object's modeltondc matrix
*/
    void Update() override;

    /*!
* \brief Draw each object that has each attribute
*/
    void Draw() override;
    void ImGuiDraw() override;

    /*!
* \brief To recieve user's input like mouse, key board
* \param SDL_Window& sdl_window
    this window
* \param SDL_Event& event
    what event
*/
    void HandleEvent(SDL_Window& sdl_window, const SDL_Event& event) override;

private:
    struct Object
    {
        unsigned    model_index = 0;
        std::string name;
        unsigned    shader_index = 0;
        color3      color{};
        vec2        scaling{ 1.0f };
        float       angle_speed = 0;
        float       angle_disp = 0;
        vec2        position{};
        mat3        model_to_ndc{};
    };

    std::vector<GLShader>      shaders;
    std::vector<GLVertexArray> models;
    std::vector<Object>        objects;

    std::unordered_map<std::string, unsigned> asset_to_index;

    struct CamInfo
    {
        Object     obj{};
        Camera     camera{};
        CameraView camera_view{};
        float      move_scalar = 0;
        float      turn_scalar = 0;
        float      strafe_scalar = 0;
        float      move_speed = 120.0f;
    } caminfo;

    struct
    {
        unsigned long long prev_time  = 0;
        unsigned long long start_time = 0;
        float              fps        = 0;
        float              DeltaTime{0};
        int                count = 0;
    } timing;

    struct
    {
        int   mouseX           = 0;
        int   mouseY           = 0;
        float mouseCamSpaceX   = 0;
        float mouseCamSpaceY   = 0;
        float mouseWorldSpaceX = 0;
        float mouseWorldSpaceY = 0;
    } environment;

private:
   /*!
* \brief window resize and set viewport
* \param SDL_Window& sdl_window
    this window
*/
    void     on_window_resized(SDL_Window& sdl_window) noexcept;
    void     update_timing() noexcept;

    /*!
 * \brief compute mouse coordinates on many aspect
 */
    void     compute_mouse_coordinates() noexcept;

    /*!
* \brief load scene file
* \param const std::filesystem::path& scene_file_path
    scene file path
*/
    void     load_scene(const std::filesystem::path& scene_file_path);

    /*!
* \brief load_mesh_and_get_id
* \param const std::filesystem::path& mesh_file_path
    mesh file path
* \return input's mesh's index
*/
    unsigned load_mesh_and_get_id(const std::filesystem::path& mesh_file_path);

    /*!
* \brief load_shader_and_get_id
* \param std::string_view name
    shader program name
* \param const std::filesystem::path& vertex_filepath
    vertex shader file path
* \param const std::filesystem::path& fragment_filepath
    fragment shader file path
* \return input's shader's index
*/
    unsigned load_shader_and_get_id(std::string_view name, const std::filesystem::path& vertex_filepath, const std::filesystem::path& fragment_filepath);
};
