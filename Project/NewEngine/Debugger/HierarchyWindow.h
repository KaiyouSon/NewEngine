#pragma once
#include "IGuiWindow.h"

class HierarchyWindow : public IGuiWindow
{
private:
	std::string mDragDropSourceObjName;
	GameObject* mCopyObj;
	bool isDrag;

private:
	void CreateGameObjectPop();
	void ShowObjectMenuItem(const std::string& label, const GameObjectType type, const bool isDrawLine = true);

	void RecursiveShowObject(GameObject* obj);
	void DragAndDropObject(GameObject* obj);
	void ParentChildCancelButton();

public:
	void DrawGuiWindow() override;
};