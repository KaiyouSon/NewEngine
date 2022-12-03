#pragma once
#include "ILayer.h"
#include "ObjectManager.h"
#include "Util.h"

template<typename T> class Singleton;

class HierarchyLayer :
	public ILayer, public Singleton<HierarchyLayer>
{
private:
	friend Singleton<HierarchyLayer>;
	bool isCreateModel;
private:
	void ShowMenuContext();
	void ShowViewProjection();
	void ShowCreateModelWindow();

	void ShowCurrentScene();
	void ShowMenuBar();
	void ShowObjList();
	void ShowRightClick();

	bool IsInTheWindow();

public:
	void Initialize() override;
	void Update() override;
};