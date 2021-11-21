
//-----------------------------------------------------------------------------
// constant buffers
//-----------------------------------------------------------------------------
cbuffer Info : register(b0)
{
    float4 psColor;
    float texture_flag; // determines if we are rendering a texture or a color cube
};

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
    if(texture_flag == 1.0)
        return ColorTexture.Sample(Sampler, ps_in.uv);
    else
        return psColor;
    
    
}