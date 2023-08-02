#pragma once
#include "NewEngine.h"

class UIManager;

class NegotiationUI
{
private:
	UIManager* mUiManager;
	std::unique_ptr<Sprite> backFrame_;
	std::unique_ptr<Sprite> button_;
	std::unique_ptr<Sprite> text_;
	std::unique_ptr<Sprite> colon_;

	float alpha_;
	bool isActive_;


private:
	void AlphaUpdate();
	void TutorialMessageUpdate();

public:
	NegotiationUI();
	void Init();
	void Update();
	void DrawFrontSprite();

public:
	void SetUIManager(UIManager* uiManager);
	void SetisActive(const bool isActive);

public:
	bool GetisActive();
};

