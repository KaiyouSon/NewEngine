#pragma once
#include "NewEngine.h"
#include "Player.h"

// カメラのインターフェース
class ICamera
{
protected:
	std::unique_ptr<Camera> mCamera;
	Player* mPlayer;
	Vec3 mLockonPos;
	Vec3 mTargetPos;
	Vec3 mTargetRot;
	bool mIsEase;

protected:
	void EaseCamera();

public:
	ICamera();
	virtual ~ICamera() {}
	virtual void Init(Player* player) = 0;
	virtual void Update() = 0;

public:
	void SetLockonPos(const Vec3 pos);
	void SetisEase(const bool isEase);

public:
	Vec3 GetLockonPos();

};
