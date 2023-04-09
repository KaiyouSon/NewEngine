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
	static const float clearColor[4];
	Transform transform;

	std::vector<CD3DX12_VIEWPORT> viewports;	// ビューポート
	std::vector<CD3DX12_RECT> scissorRects;		// シザー矩形

public:
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> buffers; //テクスチャのリソース
	std::vector<RenderTarget> renderTargets;
	DepthBuffer depthBuffer;

public:
	RenderTexture();

	void PreDrawScene();
	void PostDrawScene();
};