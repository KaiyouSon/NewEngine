#pragma once

// レンズフレアのクラス
class LensFlare
{
public:
	enum class PassType
	{
		Target,
		Mask,

		Size
	};

private:
	std::array<RenderTexture*, (uint32_t)PassType::Size> mTexes;
	std::unique_ptr<PostEffect> mPass;

public:
	LensFlare();
	void Update();
	void DrawPostEffect();
	void DrawPass(const std::function<void()>& targetDrawFunc, const std::function<void()>& maskDrawFunc);
	void PrevSceneDraw(const PassType type);
	void PostSceneDraw(const PassType type);
	void DrawDebugGui();

	void SetCenterPos(const Vec2 centerPos);
};

