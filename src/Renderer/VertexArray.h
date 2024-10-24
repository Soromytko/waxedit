#pragma once
#include <iostream>
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class VertexArray
{
protected:
	VertexArray() = default;

public:
	static VertexArray* create();
	virtual void bind() const = 0;
	virtual void unbind() const = 0;
	virtual void setIndexBuffer(IndexBuffer* indexBuffer);
	virtual void addVertexBuffer(VertexBuffer* vertexBuffer);

protected:
	std::vector<std::shared_ptr<VertexBuffer>> _vertexBuffers{};
	std::shared_ptr<IndexBuffer> _indexBuffer{};
};
