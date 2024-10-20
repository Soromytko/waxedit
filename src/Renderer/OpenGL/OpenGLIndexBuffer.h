#pragma once
#include "../IndexBuffer.h"

class OpenGlIndexBuffer : public IndexBuffer
{
	//friend class IndexBuffer;
private:
	OpenGlIndexBuffer(IndexBuffer::IndexData&& data)
public:
	void bind() override const;
	void unbind() override const;
};