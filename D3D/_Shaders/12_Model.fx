#include "00_Global.fx"
#include "00_Light.fx"

float3 LightDirection = float3(-1, -1, 1);

struct VertexModel
{
	float4 Position : Position;
	float2 Uv : Uv;
	float3 Normal : Normal;
	float3 Tangent : Tangent;
	float4 BlendIndices : BlendIndices;
	float4 BlendWeights : BlendWeights;
};

#define MAX_MODEL_TRANSFORMS 250
cbuffer CB_Bones
{
	Matrix Transforms[MAX_MODEL_TRANSFORMS];
	uint BoneIndex;
}

struct VertexOutput
{
	float4 Position : SV_Position;
	float2 Uv : Uv;
	float3 Normal : Normal;
};

VertexOutput VS(VertexModel input)
{
	VertexOutput output;
	
	World = mul(Transforms[BoneIndex], World);
	
	output.Position = WorldPosition(input.Position);
	output.Position = ViewProjection(output.Position);
	
	output.Normal = WorldNormal(input.Normal);
	output.Uv = input.Uv;
	
	return output;
}

float4 PS(VertexOutput input) : SV_Target
{
	float3 normal = normalize(input.Normal);
	float lambert = dot(normal, -LightDirection);
	float4 diffuse = DiffuseMap.Sample(LinearSampler, input.Uv);
	
	return diffuse * lambert;
}

technique11 T0
{
	P_VP(P0, VS, PS)
	P_RS_VP(P1, FillMode_WireFrame, VS, PS)
}