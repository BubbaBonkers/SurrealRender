Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

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

float4 main(VS_OUTPUT InputPixel) : SV_TARGET
{
    float4 FinalColor = float4(0, 0, 0, 0);
    FinalColor += saturate((dot((float3)DirectionalLightDirections[0], InputPixel.Normal) * DirectionalLightIntensities[0]) * DirectionalLightColors[0]);
    /*
    for (int i = 0; i < 1; i++)
    {
        FinalColor += ((saturate(dot((float3)DirectionalLightDirections[i], InputPixel.Normal) * DirectionalLightColors[i])) * DirectionalLightIntensities[i]);
    }
    */

    FinalColor = txDiffuse.Sample(samLinear, InputPixel.Texture);
    FinalColor.a = 1;

	//return float4(InputPixel.Texture, 1, 0);
    return FinalColor;
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