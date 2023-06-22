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
	std::vector<CD3DX12_VIEWPORT> viewports_;	// �r���[�|�[�g
	std::vector<CD3DX12_RECT> scissorRects_;		// �V�U�[��`

public:
	static const float sClearColor[4];
	std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> cpuHandles; //SRV�̃n���h��(CPU��)
	std::vector<D3D12_GPU_DESCRIPTOR_HANDLE> gpuHandles; //SRV�̃n���h��(GPU��)
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> buffers; //�e�N�X�`���̃��\�[�X
	std::vector<RenderTarget> renderTargets;
	DepthBuffer depthBuffer;
	Vec2 size;

public:
	void PrevDrawScene();
	void PostDrawScene();
};