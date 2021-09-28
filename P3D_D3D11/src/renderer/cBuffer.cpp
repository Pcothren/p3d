#include "cBuffer.h"
#include "p3d.h"

namespace p3d {

	cBuffer cCreateBuffer(void* data, uint32_t count, uint32_t stride, D3D11_BIND_FLAG flags)
	{
		cBuffer buffer{};
		buffer.count = count;
		buffer.stride = stride;

        // Fill in a buffer description.
        D3D11_BUFFER_DESC bufferDesc = {};
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        bufferDesc.CPUAccessFlags = 0u;

        bufferDesc.ByteWidth = stride * count;
        bufferDesc.StructureByteStride = stride;
        bufferDesc.BindFlags = flags;
        bufferDesc.MiscFlags = 0u;

        // Define the resource data.
        D3D11_SUBRESOURCE_DATA InitData = {};
        InitData.pSysMem = data;

        // Create the buffer with the device.
        GContext->graphics.device->CreateBuffer(&bufferDesc, &InitData, buffer.buffer.GetAddressOf());

		return buffer;
	}
}