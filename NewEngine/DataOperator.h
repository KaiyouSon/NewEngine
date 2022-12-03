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

	// �Q�[���E�B���h�E�f�[�^
	void SaveWindowData();
	void LoadWindowData();

	// ���f���f�[�^���X�g
	void SaveModelDataList();
	void LoadModelDataList();

	// �e�N�X�`���[���X�g
	void SaveTextureList();
	void LoadTextureList();

	// �Q�[���I�u�W�F�N�g���X�g
	void SaveObjectList();
	void LoadObjectList();

	void SaveSceneList();
	void LoadSceneList();

	bool CheckSaveData();
	void SetGameWindowParameter(const std::string& title, const Vec2& size);

	std::string GetGameWindowTitleForStorage();
	Vec2 GetGameWindowSizeForStorage();
};
