#include "VertexBufferLayout.h"

#define INT_SIZE 4
#define FLOAT_SIZE 4

static uint32_t getSizeByType(ShaderDataType type)
{
	const int intSize = 4;
	switch (type)
	{
	case ShaderDataType::int1: return INT_SIZE * 1;
	case ShaderDataType::int2: return INT_SIZE * 2;
	case ShaderDataType::int3: return INT_SIZE * 3;
	case ShaderDataType::int4: return INT_SIZE * 4;
	case ShaderDataType::float1: return FLOAT_SIZE * 1;
	case ShaderDataType::float2: return FLOAT_SIZE * 2;
	case ShaderDataType::float3: return FLOAT_SIZE * 3;
	case ShaderDataType::float4: return FLOAT_SIZE * 4;
	}
}

VertexBufferLayout::VertexBufferLayout(ShaderDataType type, bool normalized, uint32_t offset) :
	_type(type), _normalized(normalized), _offset(offset)
{
	_size = getSizeByType(type);
}

ShaderDataType VertexBufferLayout::getType() const
{
	return _type;
}

uint32_t VertexBufferLayout::getSize() const
{
	return _size;
}

uint32_t VertexBufferLayout::getOffset() const
{
	return _offset;
}

bool VertexBufferLayout::getNormalized() const
{
	return _normalized;
}

int VertexBufferLayout::getComponentCountByShaderType() const
{
	switch (_type)
	{
	case ShaderDataType::int1: case ShaderDataType::float1: return 1;
	case ShaderDataType::int2: case ShaderDataType::float2: return 2;
	case ShaderDataType::int3: case ShaderDataType::float3: return 3;
	case ShaderDataType::int4: case ShaderDataType::float4: return 4;
	}
	return 0;
}

void VertexBufferLayout::setOffset(uint32_t value)
{
	_offset = value;
}
