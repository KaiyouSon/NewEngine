#pragma once
#include "IGuiWindow.h"
#include "Util.h"

class ViewWindow : public IGuiWindow
{
private:
	Vec2 mAspect;

public:
	ViewWindow();
	void DrawGuiWindow() override;
};