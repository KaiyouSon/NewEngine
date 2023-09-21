#pragma once

class IScene
{
public:
	virtual ~IScene() {}
	virtual void Load() = 0;
	virtual void UnLoad() = 0;
	virtual void CreateInstance() = 0;
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void RenderTextureSetting() = 0;
	virtual void Draw() = 0;
	virtual void DrawDebugGui() = 0;
};