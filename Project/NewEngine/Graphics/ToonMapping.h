#pragma once
#include <functional>

// 前方宣言
class PostEffect;

// トーンマッピング
class ToonMapping
{
private:
	std::unique_ptr<PostEffect> mPostEffect;
	RenderTexture* mTexture;
	ConstantBufferData::CToonMapping mData;

public:
	ToonMapping();
	void Load();
	void Init();
	void Update();
	void DrawPass(const std::function<void()>& targetDrawFunc);
	void Draw();
	void DrawDebugGui();
};

