#pragma once
#include "RenderBase.h"
#include "MainLayer.h"
#include "SceneLayer.h"
#include "ProjectLayer.h"
#include "HierarchyLayer.h"
#include "UserLayer.h"
#include "InspectorLayer.h"
#include "Util.h"

template<typename T> class Singleton;

class GuiManager :
	public Singleton<GuiManager>
{
private:
	friend Singleton<GuiManager>;

	static const int NumFramesInFlight;

private:
	// インスタンスを取得する生ポインター
	RenderBase* renderBase;
	MainLayer* mainLayer;
	SceneLayer* sceneLayer;
	ProjectLayer* projectLayer;
	HierarchyLayer* hierarchyLayer;
	UserLayer* userLayer;
	InspectorLayer* inspectorLayer;

public:
	void Initialize();
	void Update();
	void Draw();
	void Destroy();
};