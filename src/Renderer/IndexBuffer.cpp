#include "IndexBuffer.h"
#include "OpenGL/OpenGLIndexBuffer.h"
#include "Renderer.h"

IndexBuffer::IndexBuffer(std::vector<int>&& indices)
{
	_indices = std::move(indices);
}

IndexBuffer* IndexBuffer::create(const std::vector<int>& indices)
{
	std::vector<int> value = indices;
	return create(std::move(value));
}

IndexBuffer* IndexBuffer::create(std::vector<int>&& indices)
{
	switch (Renderer::getApi())
	{
	case Renderer::Api::OpenGL: return new OpenGLIndexBuffer(std::move(indices));
	}
	return nullptr;
}