#include "cSampler.h"
#include "p3d.h"
#include <assert.h>

namespace p3d {

    cSampler cCreateSampler()
    {
        D3D11_SAMPLER_DESC sampler_desc{};
        sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
        sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
        sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
        sampler_desc.MipLODBias = 0.0f;
        sampler_desc.MaxAnisotropy = 1;
        sampler_desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
        sampler_desc.BorderColor[0] = 1.0f;
        sampler_desc.BorderColor[1] = 1.0f;
        sampler_desc.BorderColor[2] = 1.0f;
        sampler_desc.BorderColor[3] = 1.0f;
        sampler_desc.MinLOD = -FLT_MAX;
        sampler_desc.MaxLOD = FLT_MAX;


        // create and bind sampler to pixel shader
        cComPtr<ID3D11SamplerState> sampler_state;

        HRESULT hr = GContext->graphics.device->CreateSamplerState(&sampler_desc, &sampler_state);
        assert(SUCCEEDED(hr));

        cSampler sampler{};
        sampler.sampler_state = sampler_state;

        return sampler;
    }
}