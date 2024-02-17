#pragma once
#include "IGuiWindow.h"

class HierarchyWindow : public IGuiWindow
{
private:
	void CreateGameObjectPop();

public:
	void DrawGuiWindow() override;
};