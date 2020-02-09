Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

struct VS_OUTPUT
{
	float4 Position : SV_POSITION;
	float3 Normal : NORMAL;
	float2 Texture : TEXCOORD1;
};

float4 main(VS_OUTPUT InputPixel) : SV_TARGET
{
    float4 FinalColor = 0;

    FinalColor *= txDiffuse.Sample(samLinear, InputPixel.Texture);
    FinalColor.a = 1;

	return float4(InputPixel.Texture, 1, 0);
}

/*
float4 PS( PS_INPUT input) : SV_Target
{
    float4 finalColor = 0;

    //do NdotL lighting for 2 lights
    for(int i=0; i<2; i++)
    {
        finalColor += saturate( dot( (float3)vLightDir[i],input.Norm) * vLightColor[i] );
    }
    finalColor *= txDiffuse.Sample(samLinear, input.Tex);
    finalColor.a = 1;
    return finalColor;
}
*/