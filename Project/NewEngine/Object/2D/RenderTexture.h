#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "RenderTarget.h"
#include "Vec2.h"
#include <vector>
#include <memory>
#include <d3dx12.h>
#include <wrl.h>

class RenderTexture
{
private:
	std::vector<CD3DX12_VIEWPORT> mViewports;	// ビューポート
	std::vector<CD3DX12_RECT> mScissorRects;	// シザー矩形
	std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> mCpuHandles; // SRVのハンドル(CPU側)
	std::vector<D3D12_GPU_DESCRIPTOR_HANDLE> mGpuHandles; // SRVのハンドル(GPU側)

public:
	static const float sClearColor[4];
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> buffers; // テクスチャのリソース
	std::vector<RenderTarget> renderTargets;
	DepthBuffer depthBuffer;
	std::unique_ptr<Texture> depthTexture;
	Vec2 size;
	bool useDepth = false;

public:
	void PrevDrawScene();
	void PostDrawScene();

public:
	// セッター
	void SetHandleNum(const uint32_t num);
	void SetCpuHandle(const uint32_t index, const D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle);
	void SetGpuHandle(const uint32_t index, const D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle);

public: // ゲッター
	D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle(const uint32_t index);
	D3D12_GPU_DESCRIPTOR_HANDLE GetGpuHandle(const uint32_t index);
};