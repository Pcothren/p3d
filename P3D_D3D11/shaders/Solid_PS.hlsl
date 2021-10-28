
//-----------------------------------------------------------------------------
// textures
//-----------------------------------------------------------------------------
Texture2D ColorTexture : register(t0);

//-----------------------------------------------------------------------------
// samplers
//-----------------------------------------------------------------------------
SamplerState Sampler : register(s0);

struct VS_Output
{
    float4 position : SV_POSITION;
    float2 uv : UV;
};

float4 main(VS_Output ps_in) : SV_Target
{
    return ColorTexture.Sample(Sampler, ps_in.uv);
}