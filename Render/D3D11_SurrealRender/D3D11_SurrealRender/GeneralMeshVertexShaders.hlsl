struct VS_INPUT
{
	float4 Position : POSITION;
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
};

VS_OUTPUT main(VS_INPUT Input)
{
	VS_OUTPUT Output = (VS_OUTPUT)0;
	Output.Position = mul(WorldMatrix, Input.Position);
	Output.Position = mul(ViewMatrix, Output.Position);
	Output.Position = mul(ProjectionMatrix, Output.Position);
	Output.Normal = mul(float4(Input.Normal, 1), WorldMatrix).xyz;
	Output.Texture = Input.Texture;

	return Output;
}

/*
PS_INPUT VS( VS_INPUT input )
{
	PS_INPUT output = (PS_INPUT)0;
	output.Pos = mul( input.Pos, World );
	output.Pos = mul( output.Pos, View );
	output.Pos = mul( output.Pos, Projection );
	output.Norm = mul( float4( input.Norm, 1 ), World ).xyz;
	output.Tex = input.Tex;
	return output;
}
*/