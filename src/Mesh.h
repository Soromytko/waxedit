#pragma once
#include <vector>

class Mesh
{
public:
	typedef std::vector<int> IndexList;
	typedef std::vector<float> VertexList;
	typedef std::vector<float> NormalList;

	Mesh() = default;
	~Mesh() = default;

	IndexList& getIndices();
	VertexList& getVertices();
	NormalList& getNormals();

	void setIndices(const IndexList& value);
	void setIndices(IndexList&& value);
	void setVertices(const VertexList& value);
	void setVertices(VertexList&& value);
	void setNormals(const NormalList& value);
	void setNormals(NormalList&& value);

private:
	IndexList _indices{};
	VertexList _vertices{};
	NormalList _normals{};
};