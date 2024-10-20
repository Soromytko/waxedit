#pragma once
#include "../VertexBuffer.h"
#include <glad/glad.h>

class OpenGLVertexBuffer final : public VertexBuffer
{
	//friend class VertexBuffer;
public:
	OpenGLVertexBuffer(std::vector<float>&& data);
	~OpenGLVertexBuffer();
public:
	void bind() const override;
	void unbind() const override;
private:
	GLuint _bufferId{};
};