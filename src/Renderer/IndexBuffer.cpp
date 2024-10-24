#include "IndexBuffer.h"
#include "OpenGL/OpenGLIndexBuffer.h"
#include "Renderer.h"

IndexBuffer::IndexBuffer(std::vector<uint32_t>&& indices)
{
	_indices = std::move(indices);
}

IndexBuffer* IndexBuffer::create(const std::vector<uint32_t>& indices)
{
	std::vector<uint32_t> value = indices;
	return create(std::move(value));
}

IndexBuffer* IndexBuffer::create(std::vector<uint32_t>&& indices)
{
	switch (Renderer::getApi())
	{
	case Renderer::Api::OpenGL: return new OpenGLIndexBuffer(std::move(indices));
	}
	return nullptr;
}