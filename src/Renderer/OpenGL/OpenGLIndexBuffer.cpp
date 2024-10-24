#include "OpenGLIndexBuffer.h"

OpenGLIndexBuffer::OpenGLIndexBuffer(std::vector<uint32_t>&& indices) :
	IndexBuffer(std::move(indices))
{
	glCreateBuffers(1, &_bufferId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _bufferId);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		_indices.size() * sizeof(uint32_t),
		static_cast<const void*>(_indices.data()),
		GL_STATIC_DRAW
	);
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