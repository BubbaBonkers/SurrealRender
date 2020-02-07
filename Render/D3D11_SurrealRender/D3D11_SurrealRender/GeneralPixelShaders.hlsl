struct VS_OUTPUT
{
	float4 Position : SV_POSITION;
	float3 Normal : NORMAL;
	float2 Texture : TEXCOORD1;
};

float4 main(VS_OUTPUT InputPixel) : SV_TARGET
{
	return float4(InputPixel.Texture, 1, 0);
}