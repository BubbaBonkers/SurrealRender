struct InputVertex
{
	float4 Position : POSITION;
	float4 Color : COLOR;
};

struct OutputVertex
{
	float4 Position : SV_POSITION;
};

OutputVertex main(InputVertex Input)
{
	OutputVertex Output = (OutputVertex)0;
	Output.Position = Input.Position;

	return Output;
}