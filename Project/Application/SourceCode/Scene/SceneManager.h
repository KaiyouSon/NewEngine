#pragma once
#include "IScene.h"
#include "Util.h"

template<typename T> class Singleton;

class SceneManager : public Singleton<SceneManager>
{
private:
	friend Singleton<SceneManager>;
	static std::unique_ptr<IScene> sCurrentScene;

public:
	SceneManager();
	~SceneManager();

public:
	void Init();
	void Update();
	void RenderTextureSetting();
	void Draw();
	void DrawDebugGui();

	template<typename T>
	static void ChangeScene()
	{
		// ���݂̃V�[���̃A�Z�b�g���A�����[�h����
		sCurrentScene->UnLoad();

		// ���̃V�[���̃C���X�^���X���쐬
		std::unique_ptr<IScene> nextScene = std::make_unique<T>();

		// �V�[�����Ŏg���A�Z�b�g�̃��[�h
		nextScene->Load();

		// �V�[�����Ŏg���C���X�^���X����
		nextScene->CreateInstance();

		// �V�[��������
		nextScene->Init();

		sCurrentScene = std::move(nextScene);
	}
};

