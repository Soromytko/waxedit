#pragma once
#include "../VertexBuffer.h"

class OpenGLVertexBuffer : public VertexBuffer
{
	//friend class VertexBuffer;
public:
	OpenGLVertexBuffer(std::vector<float>&& data);
public:
	void bind() const override;
	void unbind() const override;
private:
	int _bufferId{};
};