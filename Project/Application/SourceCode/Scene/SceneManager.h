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
	void DrawBackSprite();
	void DrawModel();
	void DrawFrontSprite();
	void DrawRenderTexture();

	template<typename T>
	static void ChangeScene()
	{
		// ���݂̃V�[���̃A�Z�b�g���A�����[�h����
		sCurrentScene->UnLoad();

		// ���̃V�[���̃C���X�^���X���쐬
		std::unique_ptr<IScene> nextScene = std::make_unique<T>();
		nextScene->Load();	// ���[�h
		nextScene->Init();	// ������

		sCurrentScene = std::move(nextScene);
	}
};

