#pragma once
#include "IScene.h"
#include "Util.h"
#include <future>
#include <thread>

template<typename T> class Singleton;

class SceneManager : public Singleton<SceneManager>
{
private:
	enum ChangeStep
	{
		CreateInstance,
		Loading,
		End
	};

private:
	friend Singleton<SceneManager>;
	static std::unique_ptr<IScene> sCurrentScene;
	static std::unique_ptr<IScene> sNextScene;
	static bool sIsChanged;
	ChangeStep mChangeStep;

public:
	SceneManager();
	~SceneManager();

public:
	void Init();
	void Update();
	void RenderTextureSetting();
	void Draw();
	void DrawDebugGui();

public:
	static bool InitNextScene();

	template<typename T>
	static void ChangeScene()
	{
		sIsChanged = false;

		switch (GetInstance()->mChangeStep)
		{
		case CreateInstance:
		{
			// ���̃V�[���̃C���X�^���X���쐬
			sNextScene = std::make_unique<T>();
			GetInstance()->mChangeStep = Loading;
		}
		break;

		case Loading:
		{
			std::future<void> ftr = std::async(std::launch::async,
				[]()
				{
					// ���݂̃V�[���̃A�Z�b�g���A�����[�h����
					sCurrentScene->UnLoad();

					// �V�[�����Ŏg���A�Z�b�g�̃��[�h
					sNextScene->Load();

					// �V�[�����Ŏg���C���X�^���X����
					sNextScene->CreateInstance();

					// �V�[��������
					sNextScene->Init();

					GetInstance()->mChangeStep = End;
				});
		}
		break;

		case End:
		{
			sCurrentScene = std::move(sNextScene);

			// ���̂��߂̏���
			GetInstance()->mChangeStep = CreateInstance;

			// �I������̂�m�点�邽��
			sIsChanged = true;
		}
		break;

		}
	}

	static bool GetisChanged();
};

