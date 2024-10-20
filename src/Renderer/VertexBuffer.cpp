#include "VertexBuffer.h"
#include "OpenGL/OpenGlVertexBuffer.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer(std::vector<float>&& data)
{
	_data = std::move(data);
}

VertexBuffer::~VertexBuffer()
{
	
}

VertexBuffer* VertexBuffer::create(const std::vector<float>& data)
{
	std::vector<float> value = data;
	return create(std::move(value));
}

VertexBuffer* VertexBuffer::create(std::vector<float>&& data)
{
	switch (Renderer::getApi())
	{
	case Renderer::Api::OpenGL: return new OpenGLVertexBuffer(std::move(data));
	}
	return  nullptr;
}

VertexBuffer* VertexBuffer::create(const float* data, size_t size)
{
	std::vector<float> value;
	value.resize(size);
	for (size_t i = 0; i < size; i++) {
		value[i] = data[i];
	}
	return create(std::move(value));
}