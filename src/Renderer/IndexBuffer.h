#pragma once
#include <vector>

class IndexBuffer
{
protected:
	IndexBuffer(std::vector<int>&& indices);

public:
	static IndexBuffer* create(const std::vector<int>& indices);
	static IndexBuffer* create(std::vector<int>&& indices);
	virtual void bind() const = 0;
	virtual void unbind() const = 0;

protected:
	std::vector<int> _indices;

};