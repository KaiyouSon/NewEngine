#pragma once
#include "IGuiWindow.h"
#include "GameObject.h"

class InspectorWindow : public IGuiWindow
{
private:
	std::string mCurrentObjTag;
	GameObject* mCurrentObj;
	GameObject* mCopyObj;

private:
	void ShowGameObjectData();

private:
	void AddComponetButton();

public:
	void DrawGuiWindow() override;
};

