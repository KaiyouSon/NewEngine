#include "DepthBuffer.h"
#include "RenderBase.h"
#include "RenderWindow.h"
#include <cassert>

void DepthBuffer::Create(const Vec2 size)
{
	mBufferResource = std::make_unique<BufferResource>();

	HRESULT result;

	Vec2 depthResourceSize = size;
	depthResourceSize = (depthResourceSize != -1) ? size : RenderWindow::GetInstance()->GetWindowSize();

	// 繝ｪ繧ｽ繝ｼ繧ｹ縺ｮ險ｭ螳・
	D3D12_RESOURCE_DESC depthResourceDesc{};
	depthResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResourceDesc.Width = (uint32_t)depthResourceSize.x;		// 蟷・
	depthResourceDesc.Height = (uint32_t)depthResourceSize.y; // 鬮倥＆
	depthResourceDesc.DepthOrArraySize = 1;
	depthResourceDesc.Format = DXGI_FORMAT_D32_FLOAT;	// 豺ｱ蠎ｦ蛟､繝・ヵ繧ｩ繝ｫ繝・
	depthResourceDesc.SampleDesc.Count = 1;
	depthResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	depthResourceDesc.MipLevels = 1;

	// 豺ｱ蠎ｦ逕ｨ繝偵・繝励・繝ｭ繝代ユ繧｣
	D3D12_HEAP_PROPERTIES depthHeapProp{};
	depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
	// 豺ｱ蠎ｦ蛟､縺ｮ繧ｯ繝ｪ繧｢險ｭ螳・
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f;	// 豺ｱ蠎ｦ蛟､1.0f(譛螟ｧ蛟､)縺ｧ繧ｯ繝ｪ繧｢
	depthClearValue.Format = DXGI_FORMAT_D32_FLOAT;	// 豺ｱ蠎ｦ蛟､繝輔か繝ｼ繝槭ャ繝・

	// 繝ｪ繧ｽ繝ｼ繧ｹ縺ｮ逕滓・
	result = RenderBase::GetInstance()->GetDevice()->
		CreateCommittedResource(
			&depthHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&depthResourceDesc,
			D3D12_RESOURCE_STATE_DEPTH_WRITE, // 豺ｱ蠎ｦ蛟､譖ｸ縺崎ｾｼ縺ｿ縺ｫ菴ｿ逕ｨ
			&depthClearValue,
			IID_PPV_ARGS(&mBufferResource->buffer));
	assert(SUCCEEDED(result));

	mBufferResource->bufferState = D3D12_RESOURCE_STATE_DEPTH_WRITE;
}

// 繧ｲ繝・ち繝ｼ
BufferResource* DepthBuffer::GetBufferResource()
{
	return mBufferResource.get();
}
