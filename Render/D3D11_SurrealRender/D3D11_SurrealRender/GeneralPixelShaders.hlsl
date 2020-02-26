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
    float4x4 CameraWorldMatrix;
	float4 AmbientLightColor;
	float4 DirectionalLightDirections[1];
    float4 SpotLightDirections[1];
	float4 PointLightPositions[2];
    float4 SpotLightPositions[1];
	float4 DirectionalLightColors[1];
	float4 PointLightColors[2];
    float4 SpotLightColors[1];
    float4 EmissiveColor;
    float SpotLightConeRatios;
    float SpotLightIntensities;
	float DirectionalLightIntensities;
	float AmbientLightIntensity;
	float PointLightIntensities;
	float BlinnPhongIntensity;
    float WorldTime;
    float DeltaTime;
    float DiscoIntensity;
    float WavingIntensity;
    float WavingOffset;
    float WavingMovement;
    float BlueWavingIntensity;
    float BWIntensity;
};

float4 main(VS_OUTPUT InputPixel) : SV_TARGET
{
    // Get pixel color for texture.
	float4 FinalColor = txDiffuse.Sample(samLinear, InputPixel.Texture);
    float OriginalAlpha = FinalColor.a;
    
    // Check for alpha elimination.
    if(FinalColor.a < 0.05f)
    {
        discard;
    }
    
    if (BlueWavingIntensity == 1.0f)
    {
        if (FinalColor.b >= 0.6f)
        {
            float4 NextColor = txDiffuse.Sample(samLinear, float2(InputPixel.Texture.x + 1, InputPixel.Texture.y + 1));
        
            FinalColor.rg *= sin(NextColor.xyz * WavingOffset * WorldTime) * WavingMovement;
        }
    }
    
    // Add the emissive color now so lighting will be affected by the Emissive color.
    float4 E_Final = EmissiveColor * FinalColor.a;
    
    // Get the ambient lighting.
	float4 Ambient = ((AmbientLightColor) * AmbientLightIntensity);
    
    // Get the directional lighting.
	float D_LightRatio = saturate((dot(DirectionalLightDirections[0].xyz, InputPixel.Normal)) * DirectionalLightIntensities);
	float4 D_Result = D_LightRatio * DirectionalLightColors[0];
	
	// Get the spot lights.
    float3 SL_LightDir = normalize(SpotLightPositions[0].xyz - InputPixel.Wposition.xyz);
    float SL_SurfaceRatio = saturate(dot(-SL_LightDir, normalize(SpotLightDirections[0].xyz)));
    float SL_LightRatio = saturate(dot(SL_LightDir, InputPixel.Normal));// * SpotLightIntensities;
    float4 SL_Result = SpotLightColors[0] * SL_LightRatio;
    
    // [Spotlight] Cone Attenuation.
    float SL_Attenuation = 1.0f - saturate((0.9f - SL_SurfaceRatio) / (0.9f - 0.8f)); // CosInnerCone (.9) - surfaceRatio / cosineInerCone - inputPixel.light[i].cosineOuterCone (.8)
    SL_Attenuation = SL_Attenuation * SL_Attenuation;
    
    SL_Result *= SL_Attenuation;
    
    // [Spotlight] Distance Attenuation.
    //float SL_DistanceAttenuation = 1.0f - clamp(length(SpotLightPositions[0].xyz - normalize(InputPixel.Wposition.xyz)) / 100.0f, 0.0f, 1.0f);
    //SL_Result += ((SL_Result + (SL_LightRatio * SpotLightColors[0]) * SL_DistanceAttenuation) * SL_Attenuation);
    
    float4 P_Result = { 0, 0, 0, 0 };
    for (uint i = 0; i < 2; ++i)
    {
	    // Get the point lights.
        float3 P_LightDir = normalize(PointLightPositions[i].xyz - InputPixel.Wposition.xyz);
        float P_LightRatio = saturate((dot(P_LightDir.xyz, normalize(InputPixel.Normal)) * PointLightIntensities));
	
	    // Attenuation for point light.
        float P_Attenuation = 1.0f - clamp(length(PointLightPositions[i].xyz - normalize(InputPixel.Wposition.xyz)) / 100.0f, 0.0f, 1.0f);
        P_Result += (P_LightRatio * PointLightColors[i]) * P_Attenuation;
    }
	
	// Phong Specular Calculation
    float4 S_Result;
    if (BlinnPhongIntensity > 0.0f)
    {
        float3 S_ViewDir = normalize(float3(CameraWorldMatrix._41, CameraWorldMatrix._42, CameraWorldMatrix._43) - InputPixel.Wposition.xyz);
        float3 S_HalfVector = normalize((DirectionalLightDirections[0].xyz) + S_ViewDir);
        float3 S_Intensity = max(pow(clamp(dot(InputPixel.Normal, normalize(S_HalfVector)), 0, 1), BlinnPhongIntensity), 0);
        S_Result = DirectionalLightColors[0] * float4(1, 1, 1, 1) * float4(S_Intensity, 1);
    }
	
	// Wave based on position, time, and sin/cos/tan.
    float4 W_Color = (cos(WorldTime) + (sin(WorldTime * float4(InputPixel.Wposition, 1)) * float4(1, 1, 1, 1))) * DiscoIntensity;
    W_Color.w = 1.0f;
	
	// Add the ambient lighting to the object.
    FinalColor = ((E_Final + D_Result + Ambient + P_Result + S_Result + SL_Result + W_Color) * FinalColor);
    FinalColor.a = OriginalAlpha;

    if (BWIntensity >= 1.0f)
    {
        float AVG_RGB = ((FinalColor.a + FinalColor.g + FinalColor.b) / 3);
        FinalColor.rgb = float3(AVG_RGB, AVG_RGB, AVG_RGB);
    }
    
	// Return the fully lighted object.
    return FinalColor;
}