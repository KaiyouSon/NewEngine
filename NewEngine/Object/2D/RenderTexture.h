#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "RenderTarget.h"
#include "MathUtil.h"
#include <vector>
#include <memory>
#include <d3dx12.h>
#include <wrl.h>

class RenderTexture// : public Texture
{
private:
	std::vector<CD3DX12_VIEWPORT> viewports_;	// ビューポート
	std::vector<CD3DX12_RECT> scissorRects_;		// シザー矩形

public:
	static const float sClearColor[4];
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle; //SRVのハンドル(CPU側)
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle; //SRVのハンドル(GPU側)
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> buffers; //テクスチャのリソース
	std::vector<RenderTarget> renderTargets;
	DepthBuffer depthBuffer;
	Vec2 size;

public:
	void PrevDrawScene();
	void PostDrawScene();
};