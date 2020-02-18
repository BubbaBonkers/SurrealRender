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
};

float4 main(VS_OUTPUT InputPixel) : SV_TARGET
{
    // Get pixel color for texture.
	float4 FinalColor = txDiffuse.Sample(samLinear, InputPixel.Texture);
    if(FinalColor.a < 0.2f)
        discard;
    //FinalColor.a = 1;
    
    float4 E_Final = EmissiveColor * FinalColor.a;
    
    // Get the ambient lighting.
	float4 Ambient = ((AmbientLightColor) * AmbientLightIntensity);
    
    // Get the directional lighting.
	float D_LightRatio = saturate((dot(DirectionalLightDirections[0].xyz, InputPixel.Normal)) * DirectionalLightIntensities);
	float4 D_Result = D_LightRatio * DirectionalLightColors[0];
	
	// Get the spot lights.
    float SL_LightDir = normalize(SpotLightPositions[0].xyz - InputPixel.Wposition.xyz);
    float SL_SurfaceRatio = saturate(dot(-SL_LightDir, SpotLightDirections[0].xyz));
    float SL_LightRatio = saturate(saturate(dot(SL_LightDir, InputPixel.Normal)) * SpotLightIntensities);
    float SL_Attenuation = 1.0f - saturate((0.9f - SL_SurfaceRatio) / (0.9f - 0.8f)); // CosInnerCone (.9) - surfaceRatio / cosineInerCone - inputPixel.light[i].cosineOuterCone (.8)
    float4 SL_Result = saturate(saturate(SpotLightColors[0] * SL_LightRatio) * (SL_Attenuation * SL_Attenuation));
	
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
    float3 S_ViewDir = normalize(float3(CameraWorldMatrix._41, CameraWorldMatrix._42, CameraWorldMatrix._43) - InputPixel.Wposition.xyz);
    float3 S_HalfVector = normalize((DirectionalLightDirections[0].xyz) + S_ViewDir);
    float3 S_Intensity = max(pow(clamp(dot(InputPixel.Normal, normalize(S_HalfVector)), 0, 1), BlinnPhongIntensity), 0);
    float4 S_Result = DirectionalLightColors[0] * float4(1, 1, 1, 1) * float4(S_Intensity, 1);
    /*float3 S_LightVector = normalize(InputPixel.Position.xyz - PointLightPositions[0].xyz);
    float S_Reflect = reflect(InputPixel.Normal, S_LightVector);
    float S_ToCam = normalize(float3(CameraWorldMatrix._41, CameraWorldMatrix._42, CameraWorldMatrix._43) - InputPixel.Wposition.xyz);
    float S_SpecDot = saturate(dot(S_Reflect, S_ToCam));
    S_SpecDot = pow(S_SpecDot, BlinnPhongIntensity);
    float4 S_SpecColor = float4(1, 1, 1, 1) * float4(1, 1, 1, 1) * S_SpecDot;*/
	
	// Wave based on position, time, and sin/cos/tan.
    float4 W_Color = (cos(WorldTime) + (sin(WorldTime * float4(InputPixel.Wposition, 1)) * float4(1, 1, 1, 1))) * DiscoIntensity;
	
	// Add the ambient lighting to the object.
    FinalColor = ((E_Final + D_Result + Ambient + P_Result + S_Result + SL_Result + W_Color) * FinalColor); //    +S_SpecColor); // * FinalColor;

	// Next do diffuse.
	

	// Return the fully lighted object.
    return FinalColor;
}