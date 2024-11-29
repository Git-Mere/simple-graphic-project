/*!************************************************************************
\file Tutorial_1.cpp
\author Seungheon Jeon, Rudy Castan
\par DP email: seungheon.jeon\@digipen.edu
\par Course: CS200/fall
\par CS200 Tutorial 2 - Viewports and OpenGL Primitives
\date 09-13-2023
\brief
This header file create class Tutorial_1 and declare several function
**************************************************************************/

#pragma once

#include "GLShader.h"
#include "GLVertexArray.h"
#include "IProgram.h"

class Tutorial_2 : public IProgram
{
public:
    /*!
* \brief construct many thing such as shader, setting object's buffer, setting viewport
* \param int viewport_width set width.
* \param int viewport_height set height.
*/
    Tutorial_2(int viewport_width, int viewport_height);

    void Update() override;
    /*!
* \brief draw to screen.
*/
    void Draw() override;
    void ImGuiDraw() override;
    void HandleEvent(SDL_Window& sdl_window, const SDL_Event& event) override;

private:
    struct
    {
        unsigned long long prev_time  = 0;
        unsigned long long start_time = 0;
        float              fps        = 0;
        int                count      = 0;
    } timing;

    GLShader        shader;
    GLVertexArray models[5];
     int window_width;
     int window_height;
private:
    void on_window_resized(SDL_Window& sdl_window) noexcept;
    void update_timing() noexcept;
    /*!
 * \brief set viewports.
 * \param int width set width.
 * \param int height set height.
 */
    void set_viewports(int width, int height);
    /*!
 * \brief Create buffer of points model
 */
    void create_points_model();
    /*!
 * \brief Create buffer of lines model
 */
    void create_lines_model();
    /*!
 * \brief Create buffer of triangle fan model
 */
    void create_trifan_model();
    /*!
 * \brief Create buffer of triangle strip model
 */
    void create_tristrip_model();
    void create_triangle_model();
};
