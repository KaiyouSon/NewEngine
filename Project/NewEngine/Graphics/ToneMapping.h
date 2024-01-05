#pragma once
#include <functional>

// 前方宣言
class PostEffect;

// トーンマッピング
class ToneMapping
{
private:
	std::unique_ptr<PostEffect> mPostEffect;
	RenderTexture* mTexture;
	ConstantBufferData::CToneMapping mData;

public:
	ToneMapping();
	void Load();
	void Init();
	void Update();
	void DrawPass(const std::function<void()>& targetDrawFunc);
	void DrawPostEffect();
	void DrawDebugGui();
};

