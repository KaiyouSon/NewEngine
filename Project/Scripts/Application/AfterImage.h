#pragma once
#include "NewEngine.h"

class AfterImage : public Script
{
private:
	bool mIsAfterImage;
	uint32_t mAfterImageStep;

public:
	AfterImage();
	void Init() override;
	void Update() override;

public:
	std::shared_ptr<Script> CreateInstence() override;
};
