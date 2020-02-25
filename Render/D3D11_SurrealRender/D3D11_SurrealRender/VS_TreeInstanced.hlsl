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

cbuffer Instanced : register(b1)
{
    float4x4 InstanceLocations[100];
    float4x4 ViewMatrix;
    float4x4 ProjectionMatrix;
}

VS_OUTPUT main(VS_INPUT Input, uint id : SV_InstanceID )
{
    // Setup Position and Normals.
    VS_OUTPUT Output = (VS_OUTPUT)0;
    Output.Position = float4(Input.Position.xyz, 1);
    Output.Texture = Input.Texture;
    Output.Normal = mul(InstanceLocations[id], float4(Input.Normal.xyz, 0)).xyz;
    Output.Normal = normalize(Output.Normal);

	// Get the matrices to match up with object position.
    Output.Position = mul(InstanceLocations[id], Output.Position);
    Output.Wposition = Output.Position;
    Output.Position = mul(ViewMatrix, Output.Position);
    Output.Position = mul(ProjectionMatrix, Output.Position);
    
	return Output;
}