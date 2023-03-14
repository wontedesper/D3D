#include "stdafx.h"
#include "CubeDemo.h"

void CubeDemo::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(23, 0, 0);
	Context::Get()->GetCamera()->Position(62, 12, 19);

	shader = new Shader(L"05_World.fxo");

	vertexCount = 24;
	vertices = new Vertex[vertexCount];
	
	float w = 0.5f, h = 0.5f, d = 0.5f;

	//Front : -Z
	vertices[0].Position = Vector3(-w, -h, -d);
	vertices[1].Position = Vector3(-w, +h, -d);
	vertices[2].Position = Vector3(+w, -h, -d);
	vertices[3].Position = Vector3(+w, +h, -d);

	//Back : +Z
	vertices[4].Position = Vector3(-w, -h, +d);
	vertices[5].Position = Vector3(+w, -h, +d);
	vertices[6].Position = Vector3(-w, +h, +d);
	vertices[7].Position = Vector3(+w, +h, +d);


	//Top
	vertices[8].Position = Vector3(-w, +h, -d);
	vertices[9].Position = Vector3(-w, +h, +d);
	vertices[10].Position = Vector3(+w, +h, -d);
	vertices[11].Position = Vector3(+w, +h, +d);

	//Bottom
	vertices[12].Position = Vector3(-w, -h, -d);
	vertices[13].Position = Vector3(+w, -h, -d);
	vertices[14].Position = Vector3(-w, -h, +d);
	vertices[15].Position = Vector3(+w, -h, +d);

	//Left
	vertices[16].Position = Vector3(-w, -h, +d);
	vertices[17].Position = Vector3(-w, +h, +d);
	vertices[18].Position = Vector3(-w, -h, -d);
	vertices[19].Position = Vector3(-w, +h, -d);

	//Right
	vertices[20].Position = Vector3(+w, -h, -d);
	vertices[21].Position = Vector3(+w, +h, -d);
	vertices[22].Position = Vector3(+w, -h, +d);
	vertices[23].Position = Vector3(+w, +h, +d);

	//Create VertexBuffer
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(Vertex) * vertexCount;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA subResource = { 0 };
		subResource.pSysMem = vertices;

		Check(D3D::GetDevice()->CreateBuffer(&desc, &subResource, &vertexBuffer));
	}

	indexCount = 36;
	indices = new UINT[indexCount]
	{
		0, 1, 2, 2, 1, 3,		//Front
		4, 5, 6, 6, 5, 7,		//Back
		8, 9, 10, 10, 9, 11,	//Top
		12, 13, 14, 14, 13, 15, //Bottom
		16, 17, 18, 18, 17, 19, //Left
		20, 21, 22, 22, 21, 23, //Right
	};


	//Create IndexBuffer
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(UINT) * indexCount;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA subResource = { 0 };
		subResource.pSysMem = indices;

		Check(D3D::GetDevice()->CreateBuffer(&desc, &subResource, &indexBuffer));
	}


	SafeDeleteArray(vertices);
	SafeDeleteArray(indices);

	D3DXMatrixIdentity(&world);
}

void CubeDemo::Destroy()
{
	SafeDelete(shader);
	SafeRelease(vertexBuffer);
	SafeRelease(indexBuffer);
}

void CubeDemo::Update()
{
	Vector3 forward = Vector3(world._31, world._32, world._33);
	D3DXVec3Normalize(&forward, &forward);

	if (Keyboard::Get()->Press(VK_SHIFT))
	{
		if (Keyboard::Get()->Press('D'))
			rotation.y += rotationSpeed * Time::Delta();

		if (Keyboard::Get()->Press('A'))
			rotation.y -= rotationSpeed * Time::Delta();
	}
	else
	{
		if (Keyboard::Get()->Press('W'))
			position += forward * moveSpeed * Time::Delta();

		if (Keyboard::Get()->Press('S'))
			position -= forward * moveSpeed * Time::Delta();
	}

	Matrix S, R, T;
	D3DXMatrixScaling(&S, scale.x, scale.y, scale.z);
	D3DXMatrixRotationY(&R, rotation.y);
	D3DXMatrixTranslation(&T, position.x, position.y, position.z);
	world = S * R * T;

	shader->AsMatrix("World")->SetMatrix(world);
	shader->AsMatrix("View")->SetMatrix(Context::Get()->View());
	shader->AsMatrix("Projection")->SetMatrix(Context::Get()->Projection());
}

void CubeDemo::Render()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	shader->DrawIndexed(0, 2, indexCount);
}
