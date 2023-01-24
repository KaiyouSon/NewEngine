#pragma once

class IScene
{
public:
	virtual ~IScene() {}
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void DrawBackSprite() = 0;
	virtual void DrawModel() = 0;
	virtual void DrawFrontSprite() = 0;
	virtual void DrawDebugGui() = 0;
};