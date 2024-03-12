#pragma once
#include "IGuiWindow.h"
#include "Util.h"

class SceneWindow : public IGuiWindow
{
private:
	Vec2 mAspect;

private:
	void DrawPlayButton(const Vec2 buttonSize, const Vec2 windowSize);

public:
	SceneWindow();
	void DrawGuiWindow() override;
};