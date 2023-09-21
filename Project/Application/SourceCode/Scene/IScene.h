#pragma once

class IScene
{
public:
	virtual ~IScene() {}
	virtual void Load() = 0;
	virtual void UnLoad() = 0;
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void RenderTextureSetting() = 0;
	virtual void DrawBackSprite() = 0;
	virtual void DrawModel() = 0;
	virtual void DrawFrontSprite() = 0;
	virtual void DrawRenderTexture() = 0;
	virtual void DrawDebugGui() = 0;
};