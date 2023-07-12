#pragma once
#include "NewEngine.h"

class NegotiationUI
{
private:
	std::unique_ptr<Sprite> backFrame_;
	std::unique_ptr<Sprite> button_;
	std::unique_ptr<Sprite> text_;
	std::unique_ptr<Sprite> colon_;

	float alpha_;
	bool isActive_;

public:
	NegotiationUI();
	void Init();
	void Update();
	void DrawFrontSprite();

public:
	void SetisActive(const bool isActive);

public:
	bool GetisActive();
};

