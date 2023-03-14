#pragma once

class ModelRenderer
{
public:
	ModelRenderer(Shader* shader);
	~ModelRenderer();

	void Update();
	void Render();

public:
	void ReadMesh(wstring file);
	void ReadMaterial(wstring file);

	Transform* GetTransform() { return transform; }
	Model* GetModel() { return model; }

	void Pass(UINT pass);

private:
	void UpdateTransforms();

private:
	bool bRead = false;

	Shader* shader;
	Model* model;

	Transform* transform;

	Matrix transforms[MAX_MODEL_TRANSFORMS];
};