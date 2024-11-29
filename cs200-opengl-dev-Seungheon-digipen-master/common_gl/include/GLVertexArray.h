/*!************************************************************************
\file GLIndexBuffer.h
\author Seungheon Jeon, Rudy Castan
\par DP email: seungheon.jeon\@digipen.edu
\par Course: CS200/fall
\par CS200 Tutorial 1 - Initializing OpenGL Context
\date 09-09-2023
\brief
This header file declare functions about GLVertexArray
**************************************************************************/

#pragma once

#include "GLHandle.h"
#include "GLIndexBuffer.h"
#include "GLVertexBuffer.h"
#include <GL/glew.h>
#include <memory>
#include <vector>

struct GLAttributeLayout
{
    enum ComponentType : GLenum
    {
        Float = GL_FLOAT,
        Int   = GL_INT,
        Bool  = GL_BOOL
    };
    ComponentType component_type = ComponentType::Float;
    enum NumComponents : GLint
    {
        _1 = 1,
        _2 = 2,
        _3 = 3,
        _4 = 4
    };
    NumComponents component_dimension = NumComponents::_1;
    // Is this the 1st, 2nd, 3rd... (0, 1, 2...) "in attribute" of the vertex shader?
    GLuint vertex_layout_location = 0;
    // should be false for float types
    // if normalized is GL_TRUE, then integer data is normalized to the range [-1, 1]
    // If normalized is GL_FALSE then integer data is directly converted to floating point.
    GLboolean normalized = GL_FALSE;
    // byte offset to read the very 1st attribute
    // should be 0 for parallel array and struct of arrays
    // should be offsetof(Vertex, field) for array of structs
    GLuint relative_offset = 0;
    // byte offset into where we are in the buffer
    //  do we start at the beginning or somewhere in the middle for this attribute?
    GLintptr offset = 0;
    // how many bytes to step to the next attribute
    GLsizei stride = 0;
};
struct GLPrimitive
{
    enum Type : GLenum
    {
        Points        = GL_POINTS,
        Lines         = GL_LINES,
        LineLoop      = GL_LINE_LOOP,
        LineStrip     = GL_LINE_STRIP,
        Triangles     = GL_TRIANGLES,
        TriangleStrip = GL_TRIANGLE_STRIP,
        TriangleFan   = GL_TRIANGLE_FAN,
    };
};

class [[nodiscard]] GLVertexArray
{
    GLHandle                    vertex_array_handle = 0;
    std::vector<GLVertexBuffer> vertex_buffers;
    GLIndexBuffer               index_buffer;
    GLsizei                     num_indices       = 0;
    GLIndexElement::Type        indices_type      = GLIndexElement::None;
    GLPrimitive::Type           primitive_pattern = GLPrimitive::Triangles;
    GLsizei                     num_vertices      = 0;

public:
    /*!
 * \brief Explicit constructor for creating a GLVertexArray with an optional primitive pattern.
 * \param the_primitive_pattern The primitive pattern for rendering (default is GLPrimitive::Triangles).
 */
    explicit GLVertexArray(GLPrimitive::Type the_primitive_pattern = GLPrimitive::Triangles);

    /*!
     * \brief Destructor for releasing resources associated with the GLVertexArray.
     */
    ~GLVertexArray();

    GLVertexArray(const GLVertexArray&) = delete;
    GLVertexArray& operator=(const GLVertexArray&) = delete;
    GLVertexArray(GLVertexArray&& temp) noexcept;
    GLVertexArray& operator=(GLVertexArray&& temp) noexcept;

    /*!
     * \brief Enable or disable the use of this GLVertexArray for rendering.
     * \param bind If true (default), the vertex array is bound for use; if false, it is unbound.
     */
    void Use(bool bind = true) const;

    /*!
     * \brief Add a vertex buffer to the GLVertexArray with a specified layout.
     * \param vertex_buffer The vertex buffer to add.
     * \param buffer_layout The layout of attributes in the buffer.
     */
    void AddVertexBuffer(GLVertexBuffer&& vertex_buffer, std::initializer_list<GLAttributeLayout> buffer_layout);

    /*!
     * \brief Set the index buffer for the GLVertexArray.
     * \param the_indices The index buffer to set.
     */
    void SetIndexBuffer(GLIndexBuffer&& the_indices);

    /*!
     * \brief Get the OpenGL handle (ID) of the GLVertexArray.
     * \return The OpenGL handle of the vertex array.
     */
    [[nodiscard]] GLHandle GetHandle() const noexcept
    {
        return vertex_array_handle;
    }

    /*!
     * \brief Get the list of vertex buffers associated with the GLVertexArray.
     * \return A reference to the list of vertex buffers.
     */
    [[nodiscard]] const auto& GetVertexBuffers() const
    {
        return vertex_buffers;
    }

    /*!
     * \brief Get the index buffer associated with the GLVertexArray.
     * \return The index buffer.
     */
    [[nodiscard]] const auto& GetIndexBuffer() const
    {
        return index_buffer;
    }

    /*!
     * \brief Get the count of indices in the index buffer.
     * \return The number of indices.
     */
    [[nodiscard]] GLsizei GetIndicesCount() const noexcept
    {
        return num_indices;
    }

    /*!
     * \brief Get the data type of the indices in the index buffer.
     * \return The data type of the indices.
     */
    [[nodiscard]] GLIndexElement::Type GetIndicesType() const noexcept
    {
        return indices_type;
    }

    /*!
     * \brief Get the primitive pattern used for rendering.
     * \return The primitive pattern.
     */
    [[nodiscard]] GLPrimitive::Type GetPrimitivePattern() const
    {
        return primitive_pattern;
    }

    /*!
     * \brief Set the primitive pattern used for rendering.
     * \param the_primitive_pattern The primitive pattern to set.
     */
    void SetPrimitivePattern(GLPrimitive::Type the_primitive_pattern)
    {
        primitive_pattern = the_primitive_pattern;
    }

    /*!
     * \brief Get the total count of vertices across all vertex buffers.
     * \return The total vertex count.
     */
    [[nodiscard]] GLsizei GetVertexCount() const
    {
        return num_vertices;
    }

    /*!
     * \brief Set the total count of vertices across all vertex buffers.
     * \param vertex_count The total vertex count to set.
     */
    void SetVertexCount(int vertex_count)
    {
        num_vertices = vertex_count;
    }
};

/*!
 * \brief Render using indexed geometry from the specified GLVertexArray.
 * \param vertex_array The GLVertexArray containing the indexed geometry to render.
 */
void GLDrawIndexed(const GLVertexArray& vertex_array) noexcept;

/*!
 * \brief Render vertices from the specified GLVertexArray without indexing.
 * \param vertex_array The GLVertexArray containing the vertices to render.
 */
void GLDrawVertices(const GLVertexArray& vertex_array) noexcept;
