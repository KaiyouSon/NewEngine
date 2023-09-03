#pragma once
#include "MessageSign.h"
#include "Skydome.h"
#include "FieldData.h"
#include "Weed.h"

class Field
{
private:
	FieldData* mFieldData;

	std::unique_ptr<Skydome> mSkydome;
	std::array<std::unique_ptr<MessageSign>, 5> mMessageSigns;

private:
	// デバッグ用
	std::unique_ptr<Object3D> mSphere;

public:
	Field();
	void Init();
	void Update();
	void RenderTextureSetting();
	void DrawModel();
	void DrawFrontSprite();
	void DrawDebugGui();

public:
	std::array<std::unique_ptr<MessageSign>, 5>* GetMessageSigns();

	FieldData* GetFieldData();

	void SetSpherePos(const Vec3 pos);
};

