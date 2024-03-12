#pragma once
#include "NewEngine.h"

class AfterImage : public Script
{
private:
	bool isStart;


public:
	AfterImage();
	void Init();
	void Update() override;

public:
	std::shared_ptr<Script> CreateInstence() override;
};
