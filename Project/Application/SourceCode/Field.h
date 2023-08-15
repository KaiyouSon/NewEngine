#pragma once
#include "MessageSign.h"
#include "Skydome.h"
#include "RespawnPoint.h"
#include "Tree.h"

class Field
{
private:
	std::unique_ptr<Object3D> mGround;
	std::unique_ptr<Skydome> mSkydome;
	std::unique_ptr<RespawnPoint> mRespawnPoint;
	std::array<std::unique_ptr<MessageSign>, 5> mMessageSigns;
	std::vector<std::unique_ptr<Tree>> mTrees;

private:
	// �f�o�b�O�p
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
	RespawnPoint* GetRespawnPoint();
};

