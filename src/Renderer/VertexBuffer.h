#pragma once
#include <vector>
#include "VertexBufferLayout.h"

class VertexBuffer
{
protected:
	VertexBuffer(std::vector<float>&& data);

public:
	static VertexBuffer* create(const std::vector<float>& data);
	static VertexBuffer* create(std::vector<float>&& data);
	static VertexBuffer* create(const float* data, size_t size);
	virtual void bind() const = 0;
	virtual void unbind() const = 0;
	int getStride() const;
	const std::vector<VertexBufferLayout>& getLayouts() const;
	void setLayouts(const std::vector<VertexBufferLayout>& layouts);

protected:
	void calculateOffsetsAndStride();

	std::vector<float> _data;
	int _stride = 0;
	std::vector<VertexBufferLayout> _layouts{};

};