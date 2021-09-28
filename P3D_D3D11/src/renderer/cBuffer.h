#pragma once

#include <vector>
#include "cWindows.h"
#include "cGraphics.h"

namespace p3d {

	struct cBuffer
	{
		cComPtr<ID3D11Buffer> buffer;
		uint32_t              count;
		uint32_t              stride;
	};

	cBuffer cCreateBuffer(void* data, uint32_t count, uint32_t stride, D3D11_BIND_FLAG flags);
}
