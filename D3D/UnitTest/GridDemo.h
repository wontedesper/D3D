#pragma once
#include "Systems/IExecute.h"

class GridDemo : public IExecute
{
public:
	virtual void Initialize() override;
	virtual void Destroy() override;
	virtual void Update() override;
	virtual void PreRender() override {};
	virtual void Render() override;
	virtual void PostRender() override {};
	virtual void ResizeScreen() override {};

private:
	struct Vertex
	{
		Vector3 Posision; 
	};

private:
	Shader* shader;

	UINT width = 128, height = 128;

	UINT vertexCount;
	Vertex* vertices; 
	ID3D11Buffer* vertexBuffer;

	UINT indexCount;
	UINT* indices;
	ID3D11Buffer* indexBuffer;

	Matrix world;
};