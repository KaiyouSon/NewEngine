#pragma once
#include "IGuiWindow.h"

class HierarchyWindow : public IGuiWindow
{
private:
	void CreateGameObjectPop();
	void ShowObjectMenuItem(const std::string& label, const GameObjectType type, const bool isDrawLine = true);

	void RecursiveShowObject(GameObject* obj);

public:
	void DrawGuiWindow() override;
};