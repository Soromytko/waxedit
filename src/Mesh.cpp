#include "Mesh.h"

Mesh::VertexList& Mesh::getVertices()
{
	return _vertices;
}

Mesh::IndexList& Mesh::getIndices()
{
	return _indices;
}

Mesh::NormalList& Mesh::getNormals()
{
	return _normals;
}

void Mesh::setIndices(const IndexList& value)
{
	_indices = value;
}

void Mesh::setIndices(IndexList&& value)
{
	_indices = std::move(value);
}

void Mesh::setVertices(const VertexList& value)
{
	_vertices = value;
}

void Mesh::setVertices(VertexList &&value)
{
	_vertices = std::move(value);
}

void Mesh::setNormals(const NormalList& value)
{
	_normals = value;
}

void Mesh::setNormals(NormalList&& value)
{
	_normals = std::move(value);
}
