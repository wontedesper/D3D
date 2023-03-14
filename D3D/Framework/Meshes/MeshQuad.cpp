#include "Framework.h"
#include "MeshQuad.h"

MeshQuad::MeshQuad(Shader* shader)
	: Mesh(shader)
{
}

MeshQuad::~MeshQuad()
{
}

void MeshQuad::Create()
{
	float w = 0.5f;
	float h = 0.5f;

	vector<VertexMesh> v;
	v.push_back(VertexMesh(Vector3(-w, -h, 0), Vector2(0, 1), Vector3(0, 0, -1)));
	v.push_back(VertexMesh(Vector3(-w, +h, 0), Vector2(0, 0), Vector3(0, 0, -1)));
	v.push_back(VertexMesh(Vector3(+w, -h, 0), Vector2(1, 1), Vector3(0, 0, -1)));
	v.push_back(VertexMesh(Vector3(+w, +h, 0), Vector2(1, 0), Vector3(0, 0, -1)));

	vertices = new VertexMesh[v.size()];
	vertexCount = v.size();

	copy(v.begin(), v.end(), stdext::checked_array_iterator<VertexMesh*>(vertices, vertexCount));

	indexCount = 6;
	indices = new UINT[indexCount] { 0, 1, 2, 2, 1, 3 };
}
