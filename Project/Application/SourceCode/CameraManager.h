#pragma once
#include "NewEngine.h"
#include "ICamera.h"

class CameraManager
{
public:
	enum class CameraType
	{
		Default
	};
private:
	std::unique_ptr<ICamera> currentCamera_;

	std::unique_ptr<Camera> defCame_;
	Player* player_;

public:
	CameraManager();
	void Init();
	void Update();

public:
	void SetPlayer(Player* player);
};

