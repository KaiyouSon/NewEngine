#pragma once
#include "IGuiWindow.h"
#include "Util.h"

class SceneWindow : public IGuiWindow
{
private:
	Vec2 mAspect;

public:
	SceneWindow();
	void DrawGuiWindow() override;
};