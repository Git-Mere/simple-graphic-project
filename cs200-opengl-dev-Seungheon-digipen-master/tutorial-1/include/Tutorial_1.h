/*!************************************************************************
\file Tutorial_1.h
\author Seungheon Jeon, Rudy Castan
\par DP email: seungheon.jeon\@digipen.edu
\par Course: CS200/fall
\par CS200 Tutorial 1 - Initializing OpenGL Context
\date 09-09-2023
\brief
This header file create class Tutorial_1 and declare several function
**************************************************************************/

#pragma once
#include "GLVertexArray.h"
#include "IProgram.h"
#include "GLShader.h"
#include <string>
#include <vector>
class Tutorial_1 : public IProgram
{
public:

    /*!
 * \brief Constructor for Tutorial_1 class, initializing with viewport dimensions.
 * \param viewport_width The width of the viewport.
 * \param viewport_height The height of the viewport.
 */
    Tutorial_1(int viewport_width, int viewport_height);
    /*!
     * \brief Perform game update logic.
     */
    void Update();

    /*!
     * \brief Perform game graphic rendering.
     */
    void Draw();

    /*!
     * \brief Perform game graphic rendering and interface update using ImGui.
     */
    void ImGuiDraw();

    /*!
     * \brief Handle SDL events and update the game state.
     * \param sdl_window Reference to the SDL window object.
     * \param event The SDL event to be processed.
     */
    void HandleEvent(SDL_Window& sdl_window, const SDL_Event& event);

private:
    struct
    {
        unsigned long long prev_time  = 0;
        unsigned long long start_time = 0;
        double             fps        = 0;
        int                count      = 0;
    } timing;

    GLShader        shader;
    GLVertexArray   left_eye_model, right_eye_model, mouth_model;

    struct {
        GLclampf red = 0.392f;
        GLclampf green = 0.584f;
        GLclampf blue = 0.678f;
    } background;
    int color_count = 0;

    std::vector<std::pair<const char *, GLubyte const*>> settings_descriptions;
    std::vector<std::pair<const char*, GLint>> settings_descriptions2;
    std::pair<const char*, GLboolean> bul;
    std::pair<const char*, GLint*> view_dims;
    GLint store[2] = {};

private:
    /*!
     * \brief Create a buffer in a "struct of arrays" style.
     */
    void create_1buffer_struct_of_arrays_style();

    /*!
     * \brief Create a buffer in an "array of structs" style.
     */
    void create_1buffer_array_of_structs_style();

    /*!
     * \brief Create parallel buffers.
     */
    void create_parallel_buffers_style();

    /*!
     * \brief Handle window resizing events and adjust rendering accordingly.
     * \param sdl_window Reference to the SDL window object.
     */
    void on_window_resized(SDL_Window& sdl_window) noexcept;

    /*!
     * \brief Update timing information for the application.
     */
    void update_timing() noexcept;

    /*!
     * \brief Print and save OpenGL settings.
     */
    void print_and_save_opengl_settings() noexcept;

    /*!
     * \brief Set the clear color for rendering.
     */
    void set_clear_color();
};
