/*!************************************************************************
\file GLVertexBuffer.cpp
\author Seungheon Jeon
\par DP email: seungheon.jeon\@digipen.edu
\par Course: CS200/fall
\par CS200 Tutorial 1 - Initializing OpenGL Context
\date 09-09-2023
\brief
This source file implement functions about GLVertexBuffer
**************************************************************************/

#include "GLVertexBuffer.h"
#include <glCheck.h>

GLVertexBuffer::GLVertexBuffer(GLsizei size_in_bytes) : size(size_in_bytes)
{
	glCheck(glCreateBuffers(1, &buffer_handle));
	glCheck(glNamedBufferStorage(buffer_handle, size_in_bytes, nullptr, GL_DYNAMIC_STORAGE_BIT));

}

GLVertexBuffer::~GLVertexBuffer()
{
	glCheck(glDeleteBuffers(1, &buffer_handle));
}

GLVertexBuffer::GLVertexBuffer(GLVertexBuffer&& temp) noexcept : buffer_handle(temp.buffer_handle)
{
	temp.buffer_handle = 0;
}

GLVertexBuffer& GLVertexBuffer::operator=(GLVertexBuffer&& temp) noexcept
{
	std::swap(buffer_handle, temp.buffer_handle);

	return *this;
}

void GLVertexBuffer::Use(bool bind) const
{
	glCheck(glBindBuffer(GL_ARRAY_BUFFER, bind ? buffer_handle : 0));
}

void GLVertexBuffer::send_buffer_data_to_gpu(const void* data, GLsizei size_bytes, GLsizei starting_offset) const
{
	glCheck(glNamedBufferSubData(buffer_handle, starting_offset, size_bytes, data));
}
