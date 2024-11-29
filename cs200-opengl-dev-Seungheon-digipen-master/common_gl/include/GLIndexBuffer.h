/*!************************************************************************
\file GLIndexBuffer.h
\author Seungheon Jeon, Rudy Castan
\par DP email: seungheon.jeon\@digipen.edu
\par Course: CS200/fall
\par CS200 Tutorial 1 - Initializing OpenGL Context
\date 09-09-2023
\brief
This header file declare functions about IndexBuffer
**************************************************************************/

#pragma once
#include <glCheck.h>
#include "GLHandle.h"
#include <GL/glew.h>
#include <span>

struct GLIndexElement
{
    enum Type : unsigned
    {
        UInt   = GL_UNSIGNED_INT,
        UShort = GL_UNSIGNED_SHORT,
        UByte  = GL_UNSIGNED_BYTE,
        None   = 0
    };
};

class [[nodiscard]] GLIndexBuffer
{
    GLIndexElement::Type element_type   = GLIndexElement::None;
    GLsizei              count          = 0;
    GLHandle             indices_handle = 0;

public:
    /*!
     * \brief Default constructor for creating an empty GLIndexBuffer.
     */
    GLIndexBuffer() = default;

    /*!
     * \brief Constructor for creating a GLIndexBuffer from an array of unsigned integers.
     * \param indices A span containing the indices to initialize the buffer.
     */
    GLIndexBuffer(std::span<const unsigned int> indices);

    /*!
     * \brief Constructor for creating a GLIndexBuffer from an array of unsigned shorts.
     * \param indices A span containing the indices to initialize the buffer.
     */
    GLIndexBuffer(std::span<const unsigned short> indices);

    /*!
     * \brief Constructor for creating a GLIndexBuffer from an array of unsigned characters.
     * \param indices A span containing the indices to initialize the buffer.
     */
    GLIndexBuffer(std::span<const unsigned char> indices);

    /*!
     * \brief Destructor for releasing resources associated with the GLIndexBuffer.
     */
    ~GLIndexBuffer();

    GLIndexBuffer(const GLIndexBuffer&) = delete;
    GLIndexBuffer& operator=(const GLIndexBuffer&) = delete;
    GLIndexBuffer(GLIndexBuffer&& temp) noexcept;
    GLIndexBuffer& operator=(GLIndexBuffer&& temp) noexcept;

    /*!
     * \brief Enable or disable the use of this GLIndexBuffer for rendering.
     * \param bind If true (default), the index buffer is bound for use; if false, it is unbound.
     */
    void Use(bool bind = true) const;

    /*!
     * \brief Get the OpenGL handle (ID) of the GLIndexBuffer.
     * \return The OpenGL handle of the index buffer.
     */
    [[nodiscard]] GLHandle GetHandle() const noexcept
    {
        return indices_handle;
    }

    /*!
     * \brief Get the count of indices in the index buffer.
     * \return The number of indices.
     */
    [[nodiscard]] GLsizei GetCount() const noexcept
    {
        return count;
    }

    /*!
     * \brief Get the data type of the indices in the index buffer.
     * \return The data type of the indices.
     */
    [[nodiscard]] GLIndexElement::Type GetElementType() const noexcept
    {
        return element_type;
    }
private:
    /*!
     * \brief Private utility function to create an indices buffer.
     * \tparam T The type of indices.
     * \param indices A span containing the indices data.
     */
    template<typename T>
    void create_indices_buffer(std::span<T> indices)
    {
        glCreateBuffers(1, &indices_handle);
        glNamedBufferStorage(indices_handle, sizeof(T) * indices.size(), indices.data(), GL_DYNAMIC_STORAGE_BIT);
    }
};
