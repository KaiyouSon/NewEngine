#pragma once
#include "IGuiWindow.h"
#include "GameObject.h"

class InspectorWindow : public IGuiWindow
{
private:
	std::string mCurrentObjTag;
	GameObject* mCurrentObj;

private:
	void ShowGameObjectData();

private:
	void AddComponetButton();

public:
	void DrawGuiWindow() override;
};

