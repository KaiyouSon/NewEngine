#pragma once
#include "MessageSign.h"
#include "Skydome.h"

class Field
{
private:
	std::unique_ptr<Object3D> ground_;
	std::unique_ptr<MessageSign> messageSign_;
	std::unique_ptr<Skydome> skydome_;

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
	MessageSign* GetMessageSign();

};

