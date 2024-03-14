#pragma once
#include "NewEngine.h"

class GameManager : public Script
{
public:
	GameManager();
	void Init() override;
	void Update() override;

public:
	std::shared_ptr<Script> CreateInstence() override;
};
