struct VS_INPUT
{
	float3 Position : POSITION;
	float3 Normal : NORMAL;
	float2 Texture : TEXCOORD0;
};

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
    float4 SpotLightDirections[2];
    float4 PointLightPositions[1];
    float4 SpotLightPositions[2];
    float4 DirectionalLightColors[1];
    float4 PointLightColors[1];
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

cbuffer Instanced : register(b1)
{
    float4x4 InstanceLocations[100];
}

VS_OUTPUT main(VS_INPUT Input)
{   
	// Setup Position and Normals.
	VS_OUTPUT Output = (VS_OUTPUT)0;
	Output.Position = float4(Input.Position.xyz, 1);
	Output.Texture = Input.Texture;
	Output.Normal = mul(WorldMatrix, float4(Input.Normal.xyz,0)).xyz;
    Output.Normal = normalize(Output.Normal);

	// Get the matrices to match up with object position.
	Output.Position = mul(WorldMatrix, Output.Position);
    Output.Wposition = Output.Position;
	Output.Position = mul(ViewMatrix, Output.Position);
	Output.Position = mul(ProjectionMatrix, Output.Position);
    
    if (WavingIntensity > 0.0f)
    {
        Output.Position.xy += Input.Normal * sin(Input.Position.xyz * WavingOffset + WorldTime) * WavingMovement; // * WavingIntensity;
    }
    
    // Mirror surface.
    if (false)
    {
        Output.Texture += Input.Normal * sin(Input.Position.x * 10.0f + WorldTime) * 1.0f; // * WavingIntensity;
    }
    
    // Color wave.
    if (false)
    {
        Output.Normal += Input.Normal * sin(Input.Position.x * 10.0f + WorldTime) * 1.0f; // * WavingIntensity;
    }
    
    // Dimension 4
    if (false)
    {
        Output.Position.xyz *= Input.Normal * sin(Input.Position.x * 10.0f + WorldTime) * 1.0f; // * WavingIntensity;
    }
	
	// Blinn-Phong Calculation.
    /*float3 BP_Normal = Input.Normal;
    float3 BP_LightVector = normalize(PointLightPositions[0].xyz - Input.Position.xyz);
    float3 BP_ViewVector = normalize(PointLightPositions[0].xyz - float3(CameraWorldMatrix._41, CameraWorldMatrix._42, CameraWorldMatrix._43));
    float3 BP_HalfWayVector = (BP_LightVector + BP_ViewVector);
    float3 BP_Specular = pow(dot(Input.Normal, (float3(BP_HalfWayVector.xyz))), BlinnPhongIntensity);*/
	
	// Return the information for the Pixel Shader.
	return Output;
}