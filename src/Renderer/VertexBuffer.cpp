#include "VertexBuffer.h"
#include "OpenGL/OpenGlVertexBuffer.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer(std::vector<float>&& data)
{
	_data = std::move(data);
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

int VertexBuffer::getStride() const
{
	return _stride;
}

const std::vector<VertexBufferLayout>& VertexBuffer::getLayouts() const
{
	return _layouts;
}

void VertexBuffer::setLayouts(const std::vector<VertexBufferLayout>& layouts)
{
	_layouts = layouts;
	calculateOffsetsAndStride();
}

void VertexBuffer::calculateOffsetsAndStride()
{
	uint32_t offset = 0;
	_stride = 0;
	for (VertexBufferLayout& layout : _layouts)
	{
		layout.setOffset(offset);
		offset += layout.getSize();
		_stride += layout.getSize();
	}
}

