#pragma once
#include "../IndexBuffer.h"
#include <glad/glad.h>

class OpenGLIndexBuffer : public IndexBuffer
{
	//friend class IndexBuffer;
public:
	OpenGLIndexBuffer(std::vector<int>&& indices);
	~OpenGLIndexBuffer();
	void bind() const override;
	void unbind()  const override;

private:
	GLuint _bufferId{};
};