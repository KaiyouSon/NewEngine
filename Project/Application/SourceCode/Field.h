#pragma once
#include "MessageSign.h"
#include "Skydome.h"
#include "RespawnPoint.h"
#include "Tree.h"
#include "Coffin.h"
#include "FieldData.h"

class Field
{
private:
	FieldData* mFieldData;

	std::unique_ptr<Skydome> mSkydome;
	std::unique_ptr<RespawnPoint> mRespawnPoint;
	std::array<std::unique_ptr<MessageSign>, 5> mMessageSigns;
	std::vector<std::unique_ptr<Tree>> mTrees;

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
	RespawnPoint* GetRespawnPoint();

	FieldData* GetFieldData();

	void SetSpherePos(const Vec3 pos);
};

