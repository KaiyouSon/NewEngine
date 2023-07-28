#pragma once
#include "MessageSign.h"
#include "Skydome.h"

class Field
{
private:
	std::unique_ptr<Object3D> ground_;
	std::unique_ptr<Skydome> skydome_;
	std::array<std::unique_ptr<MessageSign>, 3> messageSigns_;

private:
	// デバッグ用
	std::unique_ptr<Object3D> sphere_;

public:
	Field();
	void Init();
	void Update();
	void DrawModel();
	void DrawFrontSprite();
	void DrawDebugGui();

public:
	std::array<std::unique_ptr<MessageSign>, 3>* GetMessageSigns();
};

