#pragma once
#include "DepthTexture.h"
#include "Viewport.h"
#include "ScissorRectangle.h"
#include "NewEngineSetting.h"
#include <vector>
#include <memory>
#include <wrl.h>

// レンダーテクスチャのクラス
class RenderTexture : public ITexture
{
private:
	std::vector<BufferResource> mBufferResources;
	std::vector<Viewport> mViewports;				// ビューポート
	std::vector<ScissorRectangle> mScissorRects;	// シザー矩形
	std::unique_ptr<DepthTexture> mDepthTexture;
	std::unique_ptr<DepthBuffer> mDepthBuffer;

public:
	static const float sClearColor[4];
	bool useDepth = false;

public:
	void Create(const RenderTextureSetting setting);

public:
	void PrevDrawScene();
	void PostDrawScene();

public: // ゲッター
	std::vector<BufferResource>* GetBufferResources();
	DepthTexture* GetDepthTexture();
	DepthBuffer* GetDepthBuffer();
};
