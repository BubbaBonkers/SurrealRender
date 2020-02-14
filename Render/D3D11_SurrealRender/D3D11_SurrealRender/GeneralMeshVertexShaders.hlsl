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
	float4 PointLightPositions[1];
	float4 DirectionalLightColors[1];
	float4 PointLightColors[1];
	float DirectionalLightIntensities;
	float AmbientLightIntensity;
	float PointLightIntensities;
    float BlinnPhongIntensity;
};

VS_OUTPUT main(VS_INPUT Input)
{
	// Setup Position and Normals.
	VS_OUTPUT Output = (VS_OUTPUT)0;
	Output.Position = float4(Input.Position.xyz, 1);
	Output.Texture = Input.Texture;
	Output.Normal = mul(WorldMatrix, float4(Input.Normal.xyz,0)).xyz;

	// Get the matrices to match up with object position.
	Output.Position = mul(WorldMatrix, Output.Position);
	Output.Wposition = mul(WorldMatrix, Output.Position);
	Output.Position = mul(ViewMatrix, Output.Position);
	Output.Position = mul(ProjectionMatrix, Output.Position);
	
	// Blinn-Phong Calculation.
    /*float3 BP_Normal = Input.Normal;
    float3 BP_LightVector = normalize(PointLightPositions[0].xyz - Input.Position.xyz);
    float3 BP_ViewVector = normalize(PointLightPositions[0].xyz - float3(CameraWorldMatrix._41, CameraWorldMatrix._42, CameraWorldMatrix._43));
    float3 BP_HalfWayVector = (BP_LightVector + BP_ViewVector);
    float3 BP_Specular = pow(dot(Input.Normal, (float3(BP_HalfWayVector.xyz))), BlinnPhongIntensity);*/
	
	// Return the information for the Pixel Shader.
	return Output;
}