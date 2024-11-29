/*!************************************************************************
\file GLIndexBuffer.h
\author Seungheon Jeon, Rudy Castan
\par DP email: seungheon.jeon\@digipen.edu
\par Course: CS200/fall
\par CS200 Tutorial 1 - Initializing OpenGL Context
\date 09-09-2023
\brief
This header file declare functions about vertex buffer 
**************************************************************************/

#pragma once

#include <GL/glew.h>
#include <GLHandle.h>
#include <span>


class [[nodiscard]] GLVertexBuffer
{
    GLsizei  size          = 0;
    GLHandle buffer_handle = 0;

public:
    /*!
     * \brief Explicit constructor for creating a GLVertexBuffer with a specified size in bytes.
     * \param size_in_bytes The size of the vertex buffer in bytes.
     */
    explicit GLVertexBuffer(GLsizei size_in_bytes);

    /*!
     * \brief Constructor for creating a GLVertexBuffer from data contained in a span.
     * \tparam T The type of elements in the span.
     * \tparam N The extent of the span (defaulted to dynamic_extent).
     * \param buffer_data The span containing the data to initialize the buffer.
     */
    template <typename T, auto N = std::dynamic_extent>
    GLVertexBuffer(std::span<T, N> buffer_data) : GLVertexBuffer(static_cast<GLsizei>(buffer_data.size_bytes()))
    {
        send_buffer_data_to_gpu(buffer_data.data(), size, 0);
    }

    /*!
     * \brief Destructor for releasing resources associated with the GLVertexBuffer.
     */
    ~GLVertexBuffer();

    GLVertexBuffer(const GLVertexBuffer&) = delete;
    GLVertexBuffer& operator=(const GLVertexBuffer&) = delete;
    GLVertexBuffer(GLVertexBuffer&& temp) noexcept;
    GLVertexBuffer& operator=(GLVertexBuffer&& temp) noexcept;

    /*!
     * \brief Enable or disable the use of this OpenGL buffer for rendering.
     * \param bind If true (default), the buffer is bound for use; if false, it is unbound.
     */
    void Use(bool bind = true) const;

    /*!
     * \brief Set the data in the buffer from a span of elements.
     * \tparam T The type of elements in the span.
     * \tparam N The extent of the span (defaulted to dynamic_extent).
     * \param buffer_data The span containing the data to be copied to the buffer.
     * \param starting_offset The offset in bytes where the data should be placed in the buffer (default is 0).
     */
    template <typename T, auto N = std::dynamic_extent>
    void SetData(std::span<T, N> buffer_data, GLsizei starting_offset = 0) const
    {
        send_buffer_data_to_gpu(buffer_data.data(), static_cast<GLsizei>(buffer_data.size_bytes()), starting_offset);
    }

    /*!
     * \brief Get the OpenGL handle (ID) of the buffer.
     * \return The OpenGL handle of the buffer.
     */
    [[nodiscard]] GLHandle GetHandle() const noexcept
    {
        return buffer_handle;
    }

    /*!
     * \brief Get the size of the buffer in bytes.
     * \return The size of the buffer in bytes.
     */
    [[nodiscard]] GLsizei GetSizeBytes() const noexcept
    {
        return size;
    }

private:
    /*!
 * \brief Send data to the GPU buffer.
 * \param data Pointer to the data to be sent.
 * \param size_bytes Size of the data to be sent in bytes.
 * \param starting_offset Offset in bytes where the data should be placed in the buffer.
 */
    void send_buffer_data_to_gpu(const void* data, GLsizei size_bytes, GLsizei starting_offset) const;
};
