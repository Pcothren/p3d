
cbuffer TransformCBuf : register(b0)
{
    matrix model;
    matrix modelView;
    matrix modelViewProj;
};

struct VS_Output
{
    float4 position : SV_POSITION;
    float2 uv : UV;
};

VS_Output main(float3 pos : Position, float3 n : Normal, float3 tan : Tangent, float3 bitan : Bitangent, float2 tc : Texcoord)
{
    VS_Output Output;
    Output.position = mul(modelViewProj, float4(pos, 1.0f));
    Output.uv = tc;
    return Output;
}