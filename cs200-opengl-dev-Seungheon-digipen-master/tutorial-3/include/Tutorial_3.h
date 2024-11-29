/*!************************************************************************
\file Tutorial_3.h
\author Seungheon Jeon, Rudy Castan
\par DP email: seungheon.jeon\@digipen.edu
\par Course: CS200/fall
\par CS200 Tutorials 3 - Geometric Instancing Using Model TransformsVirtual programming lab
\date 09-17-2023
\brief
This header file create class Tutorial_3 and declare several function
**************************************************************************/

#pragma once

#include "GLShader.h"
#include "GLVertexArray.h"
#include "IProgram.h"
#include "angles.h"
#include "color3.h"
#include "mat3.h"

#include <array>

class Tutorial_3 : public IProgram
{
public:
    /*!
* \brief construct many thing such as shader, setting object's buffer, setting viewport
* \param int viewport_width set width.
* \param int viewport_height set height.
*/
    Tutorial_3(int viewport_width, int viewport_height);

    /*!
* \brief update timing and update object's rotate.
*/
    void Update() override;

    /*!
* \brief make each matrix of object and send this to shader. finally draw to screen.
*/
    void Draw() override;
    void ImGuiDraw() override;
    void HandleEvent(SDL_Window& sdl_window, const SDL_Event& event) override;

private:
    constexpr static unsigned MAX_NUMBER_OBJECTS = 32'768;
    constexpr static float    WORLD_SIZE_MIN     = -640.0f;
    constexpr static float    WORLD_SIZE_MAX     = 640.0f;
    constexpr static float    OBJ_SIZE_MIN       = 50.0f;
    constexpr static float    OBJ_SIZE_MAX       = 400.0f;
    constexpr static float    ROT_SPEED_MIN      = util::to_radians(-30.0f);
    constexpr static float    ROT_SPEED_MAX      = util::to_radians(30.0f);

    struct
    {
        unsigned long long prev_time  = 0;
        unsigned long long start_time = 0;
        float              fps        = 0;
        int                count      = 0;
        unsigned long long deltatime  = 0;
    } timing;

    enum PolyMode : GLenum
    {
        Point = GL_POINT,
        Line  = GL_LINE,
        Fill  = GL_FILL
    };
    struct
    {
        int      item_current_idx = 2;
        PolyMode type             = PolyMode::Fill;
    } poly_mode;

    enum ModelType
    {
        Rectangle,
        MysteryShape,
        Count
    };

    struct Object
    {
        float     rotation_speed = 0;
        float     rotation = 0;
        vec2      scale = vec2{ 1.0f };
        vec2      position = vec2{ 0.0f };
        ModelType which_model = ModelType::Rectangle;
    };

    std::vector<Object> objects;
    GLVertexArray models[ModelType::Count];

    GLShader        shader;
    int number_of_boxes = 0;
    int number_of_mystery = 0;
    float random_r;
    float random_g;
    float random_b;
private:
    void          on_window_resized(SDL_Window& sdl_window) noexcept;
    /*!
* \brief update timing including delta time
*/
    void          update_timing() noexcept;
    /*!
* \brief create models's information such as position, color.
*/
    void          create_models();
    /*!
* \brief object 2 times according to the total number.
*/
    void          increase_number_of_objects();
    /*!
* \brief object 1/2 times according to the total number.
*/
    void          decrease_number_of_objects();
    /*!
* \brief create object that have random values.
*/
    void            create_object();
    
};
