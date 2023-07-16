#pragma once
#include "NewEngine.h"

class TutorialUI
{
private:
	std::unique_ptr<Sprite> back_;
	std::unique_ptr<Sprite> button_;
	std::unique_ptr<Sprite> colon_;
	std::unique_ptr<Sprite> text_;

	std::unique_ptr<Sprite> message_;
	std::unique_ptr<Sprite> messageSignUI_;
	Transform parent_;

	float alpha_;
	bool isActive_;

public:
	TutorialUI();
	void Init();
	void Update();
	void DrawFrontSprite();

public:
	void SetisActive(const bool isActive);

public:
	bool GetisActive();

};

