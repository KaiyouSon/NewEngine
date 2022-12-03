#pragma once
#include "MathUtil.h"
#include "Util.h"
#include <string>

template<typename T> class Singleton;

class DataOperator : public Singleton<DataOperator>
{
private:
	friend Singleton<DataOperator>;
	std::string gameWindowTitleForStorage;
	Vec2 gameWindowSizeForStorage;

	int sceneListSize = 0;

public:
	void Initialize();

	void SaveData();
	void LoadData();

	// ゲームウィンドウデータ
	void SaveWindowData();
	void LoadWindowData();

	// モデルデータリスト
	void SaveModelDataList();
	void LoadModelDataList();

	// テクスチャーリスト
	void SaveTextureList();
	void LoadTextureList();

	// ゲームオブジェクトリスト
	void SaveObjectList();
	void LoadObjectList();

	void SaveSceneList();
	void LoadSceneList();

	bool CheckSaveData();
	void SetGameWindowParameter(const std::string& title, const Vec2& size);

	std::string GetGameWindowTitleForStorage();
	Vec2 GetGameWindowSizeForStorage();
};
