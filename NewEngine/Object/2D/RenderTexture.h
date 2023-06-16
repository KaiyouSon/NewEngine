#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "Transform.h"
#include "Texture.h"
#include "RenderTarget.h"
#include "MathUtil.h"
#include <vector>
#include <memory>
#include <d3dx12.h>
#include <wrl.h>

class RenderTexture : public Texture
{
private:
	static const float sClearColor_[4];
	Transform transform_;

	std::vector<CD3DX12_VIEWPORT> viewports_;	// ビューポート
	std::vector<CD3DX12_RECT> scissorRects_;		// シザー矩形

public:
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> buffers; //テクスチャのリソース
	std::vector<RenderTarget> renderTargets;
	DepthBuffer depthBuffer;

public:
	void PrevDrawScene();
	void PostDrawScene();
};