#pragma once
#include <string>
#include <vector>

enum class ShaderDataType
{
	int1, int2, int3, int4,
	float1, float2, float3, float4,

};

class VertexBufferLayout
{
public:
	VertexBufferLayout(ShaderDataType type, bool normalized = false, uint32_t offset = 0);
	ShaderDataType getType() const;
	uint32_t getSize() const;
	uint32_t getOffset() const;
	bool getNormalized() const;
	int getComponentCountByShaderType() const;
	void setOffset(uint32_t value);

private:
	ShaderDataType _type;
	//std::string name;
	uint32_t _size;
	uint32_t _offset;
	bool _normalized;

};
