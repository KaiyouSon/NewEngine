#pragma once

// 前方宣言
class PostEffect;
class RenderTexture;

// ライトシャフトのクラス
class LightShaft
{
private:
	std::unique_ptr<PostEffect> mPostEffect;
	RenderTexture* mRenderTexture;

public:
	LightShaft();
	void Update();
	void TransferData();
	void PrevSceneDraw();
	void PostSceneDraw();
	void DrawCommands();
};