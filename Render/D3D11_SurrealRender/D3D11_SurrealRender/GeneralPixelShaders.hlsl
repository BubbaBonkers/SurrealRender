Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

struct VS_OUTPUT
{
	float4 Position : SV_POSITION;
	float3 Normal : NORMAL;
	float2 Texture : TEXCOORD1;
	float3 Wposition : WORLDPOSITION;
};

cbuffer ConstantBuffer : register(b0)	// b for Buffer, and 0 for slot 0 in GPU.
{
	float4x4 WorldMatrix;
	float4x4 ViewMatrix;
	float4x4 ProjectionMatrix;
	float4 AmbientLightColor;
	float4 DirectionalLightDirections[1];
	float4 PointLightPositions[1];
	float4 DirectionalLightColors[1];
	float4 PointLightColors[1];
	float DirectionalLightIntensities;
	float AmbientLightIntensity;
	float PointLightIntensities;
	float PadA;
};

float4 main(VS_OUTPUT InputPixel) : SV_TARGET
{
    // Get pixel color for texture.
	float4 FinalColor = txDiffuse.Sample(samLinear, InputPixel.Texture);
    FinalColor.a = 1;
    
    // Get the ambient lighting.
	float4 Ambient = ((AmbientLightColor * FinalColor) * AmbientLightIntensity);
    
    // Get the directional lighting.
	float LightRatio = saturate((dot(DirectionalLightDirections[0].xyz, InputPixel.Normal)) * DirectionalLightIntensities);
	float4 D_Result = LightRatio * DirectionalLightColors[0] * FinalColor;
	
	// Get the point lights.
    float3 LightDir = normalize(PointLightPositions[0].xyz - normalize(InputPixel.Wposition.xyz));
	float P_LightRatio = saturate((dot(LightDir.xyz, normalize(InputPixel.Normal)) * PointLightIntensities));
	float4 P_Result = P_LightRatio * PointLightColors[0] * FinalColor;
	
	// Attenuation for point light.
    float P_Attenuation = 1.0f - clamp(length(PointLightPositions[0].xyz - normalize(InputPixel.Wposition.xyz)) / 50.0f, 0.0f, 1.0f);
	
	// Add the ambient lighting to the object.
    float4 Finished = D_Result + Ambient + (P_Result * P_Attenuation);

	// Return the fully lighted object.
	return Finished;
}