#pragma once
#include "ILayer.h"
#include "Util.h"

template<typename T> class Singleton;

class UserLayer :
	public ILayer, public Singleton<UserLayer>
{
private:
	friend Singleton<UserLayer>;
	bool isExcute;

private:
	void ShowGameWindwoSetting();
	void ShowModelDataListSetting();
	void ShowTextureListSetting();
	void ShowSceneListSetting();
	void ShowExecuteButtons();

public:
	void Initialize() override;
	void Update() override;
	inline bool GetisExcute() { return isExcute; }
};