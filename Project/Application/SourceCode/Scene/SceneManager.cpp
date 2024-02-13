#include "SceneManager.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "LogoScene.h"
#include "FieldDataManager.h"
#include "TransitionManager.h"
#include "CreateManager.h"

std::unique_ptr<IScene> SceneManager::sCurrentScene = nullptr;
std::unique_ptr<IScene> SceneManager::sNextScene = nullptr;

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
	std::string path = EngineDataDirectory + "Scene/" + sceneName + "Scene.json";

	// ファイルを開く
	std::ifstream file(path);
	// ファイルが開けない場合はアサーションエラー
	if (file.fail())
	{
		assert(0);
	}

	// JSONをデシリアライズ
	nlohmann::json deserialized;
	file >> deserialized;

	std::string sceneTag = deserialized["tag"];
	std::unique_ptr<Scene> scene = std::make_unique<Scene>(sceneTag);

	// "objects"フィールドの各オブジェクトを処理
	for (nlohmann::json& object : deserialized["objects"])
	{
		scene->GetGameObjectManager()->LoadToJson(object["object"]);
	}

	file.close();

	mCurrentScene = std::move(scene);
}

void SceneManager::SaveSceneToJson(const std::string& sceneName)
{
	nlohmann::json data;
	data["tag"] = mCurrentScene->GetTag();

	nlohmann::json objectsData;
	for (const auto& obj : *mCurrentScene->GetGameObjectManager()->GetGameObjects())
	{
		nlohmann::json objectData;
		objectData["object"] = obj->SaveToJson();

		objectsData.push_back(objectData);
	}

	data["objects"] = objectsData;

	std::ofstream file(EngineDataDirectory + "Scene/" + sceneName + "Scene.json");
	file << std::setw(4) << data << std::endl;
}

SceneManager::SceneManager()
{
	Object3D::isAllLighting = true;

	CreateManager::GetInstance()->Create();

	// デバッグビルドのも実行
	ProcessAtDebugBuild([]()
		{
			sCurrentScene = std::make_unique<GameScene>();
		});

	// リリースビルドのも実行
	ProcessAtReleaseBuild([]()
		{
			sCurrentScene = std::make_unique<LogoScene>();
		});


	LoadSceneToDirectroy();

	LoadSceneToJson("Load");
}

SceneManager::~SceneManager()
{
}

void SceneManager::CreateScene(const std::string& tag)
{
	std::unique_ptr<Scene> scene = std::make_unique<Scene>(tag);

	// mapに挿入
	GetInstance()->mSceneMap.insert(std::make_pair(tag, std::move(scene)));
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
	return &GetInstance()->mSceneMap;
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
		SaveSceneToJson(mCurrentScene->GetTag());
	}

	if (mChangeStep == Changed)
	{
		mChangeStep = None;
	}

	Camera::current.Update();
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

