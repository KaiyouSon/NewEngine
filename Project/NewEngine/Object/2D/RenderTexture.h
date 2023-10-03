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
	std::vector<CD3DX12_VIEWPORT> mViewports;	// �r���[�|�[�g
	std::vector<CD3DX12_RECT> mScissorRects;	// �V�U�[��`

private:
	std::vector<BufferResource> mBufferResources;

public:
	static const float sClearColor[4];
	std::unique_ptr<Texture> depthTexture;
	DepthBuffer depthBuffer;
	Vec2 size;
	bool useDepth = false;

public:
	void PrevDrawScene();
	void PostDrawScene();

public: // �Q�b�^�[
	std::vector<BufferResource>* GetBufferResources();
};