#pragma once
#include "NewEngine.h"
#include "ICamera.h"
#include "Player.h"
#include "Boss.h"

template<typename> class Singleton;

class CameraManager : public Singleton<CameraManager>
{
public:
	enum CameraType
	{
		Default,
		Target,
	};

private:
	std::unique_ptr<ICamera> currentCamera_;
	CameraType cameraType_;
	Player* mPlayer;
	Boss* mBoss;

public:
	CameraManager();
	void Init();
	void Update();

	void ChangeCamera(const CameraType cameraType);

public:
	CameraType GetCameraType();

public:
	void SetPlayer(Player* player);
	void SetBoss(Boss* boss);

private:
	friend Singleton<CameraManager>;
};

