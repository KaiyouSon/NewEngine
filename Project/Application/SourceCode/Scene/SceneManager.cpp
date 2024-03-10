#include "SceneManager.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "LogoScene.h"
#include "FieldDataManager.h"
#include "TransitionManager.h"
#include "CreateManager.h"

std::unique_ptr<IScene> SceneManager::sCurrentScene = nullptr;
std::unique_ptr<IScene> SceneManager::sNextScene = nullptr;

Scene* gCurrentScene = nullptr;

void SceneManager::LoadSceneToDirectroy()
{
	std::filesystem::path sceneDirectroy = EngineDataDirectory + "Scene";

	for (auto& entry : std::filesystem::directory_iterator(sceneDirectroy))
	{
		const auto& path = entry.path();
		std::string name = path.filename().string();
		std::string subTag = "Scene.json";
		name.erase(name.length() - subTag.length());

		mSceneNames.push_back(name);
	}
}

void SceneManager::LoadSceneToJson(const std::string& sceneName)
{
	mCurrentScene->LoadToJson(sceneName);
}

void SceneManager::SaveSceneToJson()
{
	mCurrentScene->SaveToJson();
}

SceneManager::SceneManager()
{
	Object3D::isAllLighting = true;

	CreateManager::GetInstance()->Create();

	// デバッグビルドのも実行
	ProcessAtDebugBuild([]()
		{
			sCurrentScene = std::make_unique<TitleScene>();
		});

	// リリースビルドのも実行
	ProcessAtReleaseBuild([]()
		{
			sCurrentScene = std::make_unique<LogoScene>();
		});


	// シーンの一覧を文字列としてロード
	LoadSceneToDirectroy();

	ScriptManager::GetInstance()->Register();

	// シーンをロード
	mCurrentScene = std::make_unique<Scene>();
	gCurrentScene = mCurrentScene.get();
	LoadSceneToJson("Game");
}

SceneManager::~SceneManager()
{
}

void SceneManager::CreateScene(const std::string& sceneName)
{
	// JSONオブジェクトを作成
	nlohmann::json data;
	data["name"] = sceneName;

	// JSONをファイルに書き込む
	std::string path = EngineDataDirectory + "Scene/" + sceneName + "Scene.json";

	std::ofstream outputFile(path);
	outputFile << std::setw(4) << data << std::endl;
	outputFile.close();

	// テクスチャ用のフォルダーを作成
	std::string texDirectoryPath = AppTextureDirectory + sceneName;
	std::filesystem::create_directories(texDirectoryPath);

	// モデル用のフォルダーを作成
	std::string modelDirectoryPath = AppModelDirectory + sceneName;
	std::filesystem::create_directories(modelDirectoryPath);

	GetInstance()->mSceneNames.push_back(sceneName);
}

void SceneManager::ChangeScene(const std::string& sceneName)
{
	AddPostDrawProcessFunc([&]()
		{
			GetInstance()->LoadSceneToJson(sceneName);
		});
}

std::unordered_map<std::string, std::unique_ptr<Scene>>* SceneManager::GetSceneMap()
{
	return nullptr;
}

std::vector<std::string>* SceneManager::GetSceneNames()
{
	return &GetInstance()->mSceneNames;
}

void SceneManager::AddPostDrawProcessFunc(const std::function<void()>& func)
{
	GetInstance()->mPostDrawProcessFuncs.push_back(func);
}

void SceneManager::Init()
{
	sCurrentScene->Load();
	sCurrentScene->CreateInstance();
	sCurrentScene->Init();

	mChangeStep = None;

	mIsReturn = false;
}

void SceneManager::Update()
{
	if (Key::GetKey(DIK_LCONTROL) && Key::GetKeyDown(DIK_S))
	{
		SaveSceneToJson();
	}

	if (mChangeStep == Changed)
	{
		mChangeStep = None;
	}

	Camera::current.Update();
	Camera::debugCamera.Update();
	Camera::debugCamera.DebugUpdate();
	sCurrentScene->Update();
	TransitionManager::GetInstance()->Update();
}

void SceneManager::ExecuteCS()
{
	if (mChangeStep != None)
	{
		return;
	}

	sCurrentScene->ExecuteCS();
}

void SceneManager::DrawPass()
{
	if (mChangeStep != None)
	{
		return;
	}

	sCurrentScene->DrawPass();
}

void SceneManager::DrawDebugGui()
{
	// デバッグビルドのも実行
	ProcessAtDebugBuild([&]()
		{
			if (mChangeStep != None)
			{
				return;
			}

			sCurrentScene->DrawDebugGui();
		});
}

void SceneManager::PostDrawProcess()
{
	for (auto& func : mPostDrawProcessFuncs)
	{
		func();
	}

	// 1フレームに1回のみ実行
	mPostDrawProcessFuncs.clear();

	//if (mChangeSceneFunc == nullptr)
	//{
	//	return;
	//}

	//// 実行
	//mChangeSceneFunc();

	//// 一回のみ実行
	//mChangeSceneFunc = nullptr;
}

void SceneManager::Draw()
{
	if (mChangeStep == None)
	{
		sCurrentScene->Draw();
	}

	//mCurrentScene->DrawGameObject();
	TransitionManager::GetInstance()->DrawFrontSprite();
}

bool SceneManager::GetisLoading()
{
	return GetInstance()->mChangeStep == Loading;
}

bool SceneManager::GetisChanged()
{
	return GetInstance()->mChangeStep == Changed;
}

void SceneManager::SetChangeStepToCreateInstance()
{
	GetInstance()->mIsReturn = false;
	//GetInstance()->mChangeStep = None;
}

