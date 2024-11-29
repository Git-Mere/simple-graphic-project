/*!************************************************************************
\file GLTexture.cpp
\author Seungheon Jeon, Rudy Castan
\par DP email: seungheon.jeon\@digipen.edu
\par Course: CS200/fall
\par CS200 Tutorial 5 - Texture MappingVirtual programming lab
\date 10-05-2023
\brief
This source file make GLTexture class and define function of this
**************************************************************************/

#include "GLTexture.h"

GLTexture::~GLTexture() noexcept
{
	delete_texture();
}

GLTexture::GLTexture(GLTexture&& other) noexcept : texture_handle(other.texture_handle), width(other.width), height(other.height),
	filtering(other.filtering), wrapping(other.wrapping) 
{
	other.texture_handle = 0;
	other.width = 0;
	other.height = 0;
}

GLTexture& GLTexture::operator=(GLTexture&& other) noexcept
{
    if (this != &other) {
        delete_texture();

        texture_handle = other.texture_handle;
        width = other.width;
        height = other.height;
        filtering = other.filtering;
        wrapping = other.wrapping;

        other.texture_handle = 0;
        other.width = 0;
        other.height = 0;
    }
    return *this;
}

bool GLTexture::LoadFromFileImage(const std::filesystem::path& image_filepath, bool flip_vertical) noexcept
{
    int img_channels = 32;

    stbi_set_flip_vertically_on_load(flip_vertical);
    unsigned char* image_data = stbi_load(image_filepath.string().c_str(), &width, &height, &img_channels, 4); // 4 for RGBA
    bool success = LoadFromMemory(width, height, reinterpret_cast<RGBA*>(image_data)); // Assuming the function signature and the needed parameters
    stbi_image_free(image_data);

    return success;
}

bool GLTexture::LoadFromMemory(int image_width, int image_height, const RGBA* colors) noexcept
{
    delete_texture();

    width = image_width;
    height = image_height;

    GLuint tex_handle;
    glCheck(glCreateTextures(GL_TEXTURE_2D, 1, &tex_handle));

    constexpr GLsizei ONE_TEXTURE_LEVEL = 1;
    glCheck(glTextureStorage2D(tex_handle, ONE_TEXTURE_LEVEL, GL_RGBA8, width, height));

    constexpr GLint   FIRST_LEVEL = 0;
    constexpr GLsizei OFFSET_X = 0, OFFSET_Y = 0;
    glCheck(glTextureSubImage2D(tex_handle, FIRST_LEVEL, OFFSET_X, OFFSET_Y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, colors));

    SetFiltering(Filtering::NearestPixel);
    SetWrapping(Wrapping::Repeat);

    texture_handle = tex_handle;
	return true;
}

void GLTexture::UseForSlot(unsigned int texture_unit) const noexcept
{
    glCheck(glBindTextureUnit(texture_unit, texture_handle));
}

void GLTexture::SetFiltering(Filtering how_to_filter) noexcept
{
    if (texture_handle == 0)
        return;

    filtering = how_to_filter;

    glCheck(glTextureParameteri(texture_handle, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(filtering)));
    glCheck(glTextureParameteri(texture_handle, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(filtering)));
}

void GLTexture::SetWrapping(Wrapping how_to_wrap, Coordinate coord) noexcept
{
    if (texture_handle == 0)
        return;

    GLenum target = GL_TEXTURE_2D;
    GLenum wrapping_mode = static_cast<GLenum>(how_to_wrap);

    if (coord == Coordinate::S) {
        target = GL_TEXTURE_WRAP_S;
    }
    else if (coord == Coordinate::T) {
        target = GL_TEXTURE_WRAP_T;
    }

    glCheck(glTextureParameteri(texture_handle, target, wrapping_mode));
}

void GLTexture::delete_texture() noexcept
{
    glCheck(glDeleteTextures(1, &texture_handle));
	texture_handle = 0; // so we don't accidentally double it
	width = 0;
	height = 0;
}
