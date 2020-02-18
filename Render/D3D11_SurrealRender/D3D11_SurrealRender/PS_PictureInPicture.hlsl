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

cbuffer ConstantBuffer : register(b0) // b for Buffer, and 0 for slot 0 in GPU.
{
    float4x4 CameraWorldMatrix;
    float WorldTime;
    float DeltaTime;
};

float4 main(VS_INPUT Input) : SV_TARGET
{
    VS_OUTPUT Output;
    Output.Position.w = 1.0f;
    
    return float4(0, 0, 0, 1.0f);
}