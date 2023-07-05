#pragma once
#include "NewEngine.h"
#include "Player.h"

class CameraManager
{
public:
	enum class CameraType
	{
		Default
	};
private:
	Player* player_;

private:
	std::unique_ptr<Camera> defCame_;
	float yaw_;
	float pitch_;
	float assistYaw_;
	float targetYaw_;

	Vec3 target_;

private:
	void DefaultCameraUpdate();

public:
	CameraManager();
	void Init();
	void Update();

public:
	void SetPlayer(Player* player);
};

