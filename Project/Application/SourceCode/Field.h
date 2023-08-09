#pragma once
#include "MessageSign.h"
#include "Skydome.h"
#include "RespawnPoint.h"

class Field
{
private:
	std::unique_ptr<Object3D> mGround;
	std::unique_ptr<Skydome> mSkydome;
	std::unique_ptr<RespawnPoint> mRespawnPoint;
	std::array<std::unique_ptr<MessageSign>, 5> mMessageSigns;

private:
	// デバッグ用
	std::unique_ptr<Object3D> mSphere;

public:
	Field();
	void Init();
	void Update();
	void DrawModel();
	void DrawFrontSprite();
	void DrawDebugGui();

public:
	std::array<std::unique_ptr<MessageSign>, 5>* GetMessageSigns();
};

