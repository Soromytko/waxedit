#include "VertexArray.h"
#include "Renderer.h"
#include "OpenGL/OpenGLVertexArray.h"

VertexArray* VertexArray::create()
{
	switch (Renderer::getApi())
	{
	case Renderer::Api::OpenGL: return new OpenGLVertexArray();
	}
	return nullptr;
}

void VertexArray::setIndexBuffer(IndexBuffer* indexBuffer)
{
	_indexBuffer.reset(indexBuffer);
}

void VertexArray::addVertexBuffer(VertexBuffer* vertexBuffer)
{
	std::shared_ptr<VertexBuffer> p;
	p.reset(vertexBuffer);
	_vertexBuffers.push_back(std::move(p));
}
