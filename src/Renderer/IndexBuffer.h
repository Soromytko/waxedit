#pragma once
#include <vector>

class IndexBuffer
{
public:
	typedef std::vector<unsigned> IndexList;

protected:
	IndexBuffer(IndexList&& vertices);

public:
	static IndexBuffer* create(const IndexList& vertices);
	static IndexBuffer* create(IndexList&& vertices);
	virtual void bind() const = 0;
	virtual void unbind() const = 0;

protected:
	IndexList _indices;

};