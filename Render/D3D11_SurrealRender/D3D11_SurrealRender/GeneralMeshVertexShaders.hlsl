struct VS_INPUT
{
	float3 Position : POSITION;
	float3 Normal : NORMAL;
	float2 Texture : TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 Position : SV_POSITION;
	float3 Normal : NORMAL;
	float2 Texture : TEXCOORD1;
};

cbuffer ConstantBuffer : register(b0)	// b for Buffer, and 0 for slot 0 in GPU.
{
	float4x4 WorldMatrix;
	float4x4 ViewMatrix;
	float4x4 ProjectionMatrix;
	float4 DirectionalLightDirections[1];
	float4 DirectionalLightColors[1];
	float DirectionalLightIntensities[1];
};

VS_OUTPUT main(VS_INPUT Input)
{
	/*VS_OUTPUT Output = (VS_OUTPUT)0;
	Output.Position = mul(WorldMatrix, Input.Position);
	Output.Position = mul(ViewMatrix, Output.Position);
	Output.Position = mul(ProjectionMatrix, Output.Position);
	Output.Normal = mul(float4(Input.Normal, 1), WorldMatrix).xyz;
	Output.Texture = Input.Texture;*/

	VS_OUTPUT Output = (VS_OUTPUT)0;
	Output.Position = float4(Input.Position.xyz, 1);
	Output.Texture = Input.Texture;
	Output.Normal = mul(WorldMatrix, float4(Input.Normal.xyz,0)).xyz;

	Output.Position = mul(WorldMatrix, Output.Position);
	Output.Position = mul(ViewMatrix, Output.Position);
	Output.Position = mul(ProjectionMatrix, Output.Position);

	return Output;
}