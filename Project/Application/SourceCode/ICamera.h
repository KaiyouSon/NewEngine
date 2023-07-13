#pragma once
#include "NewEngine.h"
#include "Player.h"

class ICamera
{
protected:
	std::unique_ptr<Camera> camera_;
	Player* player_;
	Vec3 lockonPos_;
	Vec3 targetPos_;
	Vec3 targetRot_;
	bool isEaseEnd;

protected:
	void EaseCamera();

public:
	ICamera();
	virtual ~ICamera() {}
	virtual void Init(Player* player) = 0;
	virtual void Update() = 0;

public:
	void SetLockonPos(const Vec3 pos);

public:
	Vec3 GetLockonPos();

};

