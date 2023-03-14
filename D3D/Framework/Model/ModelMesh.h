#pragma once

//------------------------------------------------------------
//ModelBone
//------------------------------------------------------------
class ModelBone
{
public:
	friend class Model;

private:
	ModelBone();
	~ModelBone();

public:
	wstring Name() { return name; }

	int ParentIndex() { return parentIndex; }
	ModelBone* Parent() { return parent; }

	Matrix& Transform() { return transform; }
	void Transform(Matrix& matrix) { transform = matrix; }

	vector<ModelBone*>& Children() { return children; }

private:
	int index;
	wstring name;

	int parentIndex;
	ModelBone* parent;

	Matrix transform;
	vector<ModelBone*> children;
};

//------------------------------------------------------------
//ModelMesh
//------------------------------------------------------------
class ModelMesh
{
public:
	friend class Model;

private:
	ModelMesh();
	~ModelMesh();

	void Binding(Model* model);

public:
	void Pass(UINT val);

	void SetShader(Shader* shader);

	void Update();
	void Render();

	int BoneIndex() { return boneIndex; }
	class ModelBone* Bone() { return bone; }

	void Transforms(Matrix* transforms); //BoneTransform
	void SetTransform(Transform* transform); //ActorTransform

private:
	struct BoneDesc
	{
		Matrix Transforms[MAX_MODEL_TRANSFORMS];

		UINT BoneIndex;
		float Padding[3];
	} boneDesc;

private:
	Shader* shader;

	Transform* transform = nullptr;
	PerFrame* perFrame = nullptr;

	int boneIndex;
	ModelBone* bone;

	VertexBuffer* vertexBuffer;
	Model::VertexModel* vertices;
	UINT vertexCount;

	IndexBuffer* indexBuffer;
	UINT* indices;
	UINT indexCount;

	ConstantBuffer* boneBuffer;
	ID3DX11EffectConstantBuffer* sBoneBuffer;

	vector<class ModelMeshPart*> meshParts;
};

//------------------------------------------------------------
//ModelMeshPart
//------------------------------------------------------------
class ModelMeshPart
{
public:
	friend class Model;
	friend class ModelMesh;

private:
	ModelMeshPart();
	~ModelMeshPart();

	void Update();
	void Render();

	void Binding(Model* model);
	void SetShader(Shader* shader);

	void Pass(UINT val) { pass = val; }

private:
	Shader* shader;
	UINT pass = 0;

	Material* material = nullptr;
	wstring materialName;

	UINT startVertex;
	UINT vertexCount;

	UINT startIndex;
	UINT indexCount;
};