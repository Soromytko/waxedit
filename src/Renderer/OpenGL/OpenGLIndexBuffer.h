#pragma once
#include "../IndexBuffer.h"
#include <glad/glad.h>

class OpenGLIndexBuffer : public IndexBuffer
{
public:
	OpenGLIndexBuffer(std::vector<uint32_t>&& indices);
	~OpenGLIndexBuffer();
	void bind() const override;
	void unbind()  const override;

private:
	GLuint _bufferId{};
};