#pragma once
#include <vector>

class VertexBuffer
{
protected:
	VertexBuffer(std::vector<float>&& data);
	virtual ~VertexBuffer();

public:
	static VertexBuffer* create(const std::vector<float>& data);
	static VertexBuffer* create(std::vector<float>&& data);
	static VertexBuffer* create(const float* data, size_t size);
	virtual void bind() const = 0;
	virtual void unbind() const = 0;

protected:
	std::vector<float> _data;

};