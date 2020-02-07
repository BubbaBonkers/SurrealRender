struct InputVertex
{
	float4 Position : POSITION;
	float4 Color : COLOR;
};

struct OutputVertex
{
	float4 Position : SV_POSITION;
	float4 Color : COLOR;
};

cbuffer ConstantBuffer : register(b0)	// b for Buffer, and 0 for slot 0 in GPU.
{
	float4x4 WorldMatrix;
	float4x4 ViewMatrix;
	float4x4 ProjectionMatrix;
};

OutputVertex main(InputVertex Input)
{
	OutputVertex Output = (OutputVertex)0;
	Output.Position = Input.Position;
	Output.Color = Input.Color;

	Output.Position = mul(WorldMatrix, Output.Position);
	Output.Position = mul(ViewMatrix, Output.Position);
	Output.Position = mul(ProjectionMatrix, Output.Position);

	return Output;
}