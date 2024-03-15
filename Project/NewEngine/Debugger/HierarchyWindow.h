#pragma once
#include "IGuiWindow.h"

class HierarchyWindow : public IGuiWindow
{
private:
	bool isDrag;
	std::string mDragDropSourceObjName;

private:
	void CreateGameObjectPop();
	void ShowObjectMenuItem(const std::string& label, const GameObjectType type, const bool isDrawLine = true);

	void RecursiveShowObject(GameObject* obj);
	void DragAndDropObject(GameObject* obj);

public:
	void DrawGuiWindow() override;
};