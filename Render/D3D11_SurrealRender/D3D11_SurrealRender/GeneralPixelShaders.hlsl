struct OutputVertex
{
	float4 Position : SV_POSITION;
	float4 Color : COLOR;
};

float4 main(OutputVertex InputPixel) : SV_TARGET
{
	return InputPixel.Color;
}