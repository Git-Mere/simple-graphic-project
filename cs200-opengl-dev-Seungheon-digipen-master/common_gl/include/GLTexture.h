/*!************************************************************************
\file GLTexture.h
\author Seungheon Jeon, Rudy Castan
\par DP email: seungheon.jeon\@digipen.edu
\par Course: CS200/fall
\par CS200 Tutorial 5 - Texture MappingVirtual programming lab
\date 10-05-2023
\brief
This header file make GLTexture class and declare function of this
**************************************************************************/

#include <array>
#include <filesystem>
#include <stb_image.h>

#include "GL/glew.h"
#include "GLHandle.h"
#include "glCheck.h"

#pragma once

class [[nodiscard]] GLTexture
{
public:
    GLTexture() = default;
    ~GLTexture() noexcept;

    GLTexture(const GLTexture& other) = delete;
    /*!
* \brief move construct
* \param const GLTexture& other
*   source object
*/
    GLTexture(GLTexture&& other) noexcept;
    GLTexture& operator=(const GLTexture& other) = delete;
    /*!
* \brief move assignment operator
* \param const GLTexture&& other
*   source object
*/
    GLTexture& operator=(GLTexture&& other) noexcept;

    /*!
* \brief get information from input file(png, jpg...) and return function's success
* \param const std::filesystem::path& image_filepath
*   file name
* \param bool flip_vertical = true
*   method of up-side down file
* \return
*   this function's success
*/
    [[nodiscard]] bool LoadFromFileImage(const std::filesystem::path& image_filepath, bool flip_vertical = true) noexcept;

    using RGBA = unsigned int;
    static_assert(sizeof(RGBA) == 4, "RGBA should be the same as 4 bytes, so we can easily treat it as an RGBA int.");

    /*!
* \brief Actually make texture object and send the object's memory to GPU
* \param int image_width
*   file's width
* \param int image_height
*   file's height
* \param const RGBA* colors
*   array of pixel's data 
* \return
*   this function's success
*/
    [[nodiscard]] bool LoadFromMemory(int image_width, int image_height, const RGBA* colors) noexcept;

    /*!
* \brief Bind texture
* \param unsigned int texture_unit
*   texture's unit number
*/
    void UseForSlot(unsigned int texture_unit) const noexcept;

    [[nodiscard]] unsigned int GetHandle() const noexcept
    {
        return texture_handle;
    }
    [[nodiscard]] int GetWidth() const noexcept
    {
        return width;
    }
    [[nodiscard]] int GetHeight() const noexcept
    {
        return height;
    }
    enum Filtering : GLint
    {
        NearestPixel = GL_NEAREST,
        Linear       = GL_LINEAR
    };
    /*!
* \brief Set filterling
* \param Filtering how_to_filter
*   applying filter
*/
    void                    SetFiltering(Filtering how_to_filter) noexcept;
    [[nodiscard]] Filtering GetFiltering() const noexcept
    {
        return filtering;
    }
    enum Coordinate
    {
        S,
        T,
        Both
    };

    enum Wrapping : GLint
    {
        Repeat            = GL_REPEAT,
        ClampToEdge       = GL_CLAMP_TO_EDGE,
        ClampToBorder     = GL_CLAMP_TO_BORDER,
        MirroredRepeat    = GL_MIRRORED_REPEAT,
        MirrorClampToEdge = GL_MIRROR_CLAMP_TO_EDGE
    };

    /*!
* \brief Set wrapping
* \param Filtering how_to_wrap
*   applying wrapping
* \param Coordinate coord
*   applying range
*/
    void SetWrapping(Wrapping how_to_wrap, Coordinate coord = Coordinate::Both) noexcept;

    [[nodiscard]] std::array<Wrapping, 2> GetWrapping() const noexcept
    {
        return wrapping;
    }

private:
    /*!
* \brief delete texture
*/
    void delete_texture() noexcept;

private:
    GLHandle                texture_handle = 0;
    int                     width          = 0;
    int                     height         = 0;
    Filtering               filtering      = Filtering::NearestPixel;
    std::array<Wrapping, 2> wrapping       = {Wrapping::Repeat, Wrapping::Repeat};
};
