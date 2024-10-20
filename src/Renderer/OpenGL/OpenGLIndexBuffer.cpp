#include "OpenGLIndexBuffer.h"

OpenGLIndexBuffer::OpenGLIndexBuffer(std::vector<int>&& indices) :
	IndexBuffer(std::move(indices))
{
	glCreateBuffers(1, &_bufferId);
	glBufferData(GL_ARRAY_BUFFER, _indices.size(), _indices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, _bufferId);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer()
{
	glDeleteBuffers(1, &_bufferId);
}

void OpenGLIndexBuffer::bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _bufferId);
}

void OpenGLIndexBuffer::unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}