#include "OpenGLVertexBuffer.h"
#include <glad/glad.h>

OpenGLVertexBuffer::OpenGLVertexBuffer(std::vector<float>&& data) :
	VertexBuffer(std::move(data))
{
	glCreateBuffers(1, &_bufferId);
	glBindBuffer(GL_ARRAY_BUFFER, _bufferId);
	glBufferData(
		GL_ARRAY_BUFFER,
		_data.size() * sizeof(float),
		static_cast<const void*>(_data.data()),
		GL_STATIC_DRAW
	);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer()
{
	glDeleteBuffers(1, &_bufferId);
}

void OpenGLVertexBuffer::bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, _bufferId);
}

void OpenGLVertexBuffer::unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}