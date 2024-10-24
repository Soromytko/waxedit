#pragma once
#include <vector>

class IndexBuffer
{
protected:
	IndexBuffer(std::vector<uint32_t>&& indices);

public:
	static IndexBuffer* create(const std::vector<uint32_t>& indices);
	static IndexBuffer* create(std::vector<uint32_t>&& indices);
	virtual void bind() const = 0;
	virtual void unbind() const = 0;

protected:
	std::vector<uint32_t> _indices;

};