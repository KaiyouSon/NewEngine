#pragma once
#include "NewEngine.h"
#include "Player.h"

class ICamera
{
protected:
	std::unique_ptr<Camera> camera_;
	Player* player_;

public:
	virtual ~ICamera() {}
	virtual void Init(Player* player) = 0;
	virtual void Update() = 0;

};

