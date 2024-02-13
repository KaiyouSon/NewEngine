#pragma once
#include "GameObject.h"

class InspectorWindow
{
private:
	std::string mCurrentObjTag;
	GameObject* mCurrentObj;

private:
	void ShowGameObjectData();

private:
	void AddComponetButton();

public:
	void DrawDebugGui();
};

