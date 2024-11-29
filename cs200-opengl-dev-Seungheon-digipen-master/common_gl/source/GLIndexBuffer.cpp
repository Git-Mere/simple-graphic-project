/*!************************************************************************
\file GLIndexBuffer.cpp
\author Seungheon Jeon
\par DP email: seungheon.jeon\@digipen.edu
\par Course: CS200/fall
\par CS200 Tutorial 1 - Initializing OpenGL Context
\date 09-09-2023
\brief
This source file implement functions about IndexBuffer
**************************************************************************/

#include "GLIndexBuffer.h"
#include <glCheck.h>

GLIndexBuffer::GLIndexBuffer(std::span<const unsigned char> indices)
{
	create_indices_buffer(indices);
	count = static_cast<GLsizei>(indices.size());
	element_type = GLIndexElement::UByte;
}

GLIndexBuffer::GLIndexBuffer(std::span<const unsigned short> indices)
{
	create_indices_buffer(indices);
	count = static_cast<GLsizei>(indices.size());
	element_type = GLIndexElement::UShort;
}

GLIndexBuffer::GLIndexBuffer(std::span<const unsigned int> indices)
{
	create_indices_buffer(indices);
	count = static_cast<GLsizei>(indices.size());
	element_type = GLIndexElement::UInt;
}

GLIndexBuffer::~GLIndexBuffer()
{
	glCheck(glDeleteBuffers(1, &indices_handle));
}

GLIndexBuffer::GLIndexBuffer(GLIndexBuffer&& temp) noexcept : element_type(temp.element_type), count(temp.count), indices_handle(temp.indices_handle)
{
	temp.element_type = GLIndexElement::None;
	temp.count = 0;
	temp.indices_handle = 0;
}

GLIndexBuffer& GLIndexBuffer::operator=(GLIndexBuffer&& temp) noexcept
{
	std::swap(element_type, temp.element_type);
	std::swap(count, temp.count);
	std::swap(indices_handle, temp.indices_handle);

	return *this;
}

void GLIndexBuffer::Use(bool bind) const
{
	glCheck(glBindBuffer(GL_ARRAY_BUFFER, bind ? indices_handle : 0));

}


