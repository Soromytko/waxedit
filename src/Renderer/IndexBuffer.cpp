#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(IndexList&& indices)
{
	_indices = std::move(indices);
}

IndexBuffer* IndexBuffer::create(const IndexList& indices)
{
	IndexList value = indices;
	return create(std::move(value));
}

IndexBuffer* IndexBuffer::create(IndexList&& indices)
{
	return new IndexBuffer(std::move(indices));
}