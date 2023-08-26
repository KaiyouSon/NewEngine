#include "NewEngine.h"

#include "RenderTexture.h"
#include "LoadManager.h"
#include "DebugManager.h"
#include <wrl.h>
using namespace Microsoft::WRL;

NewEngine::NewEngine(const NewEngineSetting& setting) :
	mSetting(setting),
	mRenderWindow(RenderWindow::GetInstance().get()),
	mRenderBase(RenderBase::GetInstance())
{
}

NewEngine::~NewEngine()
{
	Gui::Destroy();
	RenderWindow::GetInstance()->TerminateGameWindow();		// ウィンドウクラスを登録解除
	SoundManager::Destroy();

	RenderBase::Destroy();
}

void NewEngine::Setting()
{
	// ウィンドウタイトル
	mRenderWindow->SetWindowTitle(mSetting.windowTitle);

	// ウィンドウサイズ
	mRenderWindow->SetWindowSize(mSetting.windowSize);

	// 背景色
	mRenderBase->sClearColor[0] = mSetting.bgColor.To01().r;
	mRenderBase->sClearColor[1] = mSetting.bgColor.To01().g;
	mRenderBase->sClearColor[2] = mSetting.bgColor.To01().b;
}

void NewEngine::Init()
{
	Setting();

	// ウィンドウ生成
	mRenderWindow->CreateGameWindow();

	// RenderBaseの初期化
	mRenderBase->Init();

	// ランドの初期化
	Random::Init();

	//GUIの初期化
	Gui::Init();

	// サウンドマネージャの初期化
	SoundManager::Init();

	// フレームレートの初期化
	FrameRate::GetInstance()->Init(mSetting.frameRate);

	// インプットマネージャの初期化
	InputManager::GetInstance()->Init();

	// デバッグマネージャーの初期化
	DebugManager::GetInstance()->Init();

	// ロードマネージャのロード
	LoadManager::GetInstance()->Load();

	//　ロード終了チェック
	bool isLoaded = LoadManager::GetInstance()->GetisLoaded();
	if (isLoaded == true)
	{
		// コライダードロワーのロードと初期化
		ColliderDrawer::GetInstance()->Load();
		ColliderDrawer::GetInstance()->Init();

		// シーンマネージャーの初期化
		SceneManager::GetInstance()->Init();
	}
}

void NewEngine::Update()
{
	//　ロード終了チェック
	bool isLoaded = LoadManager::GetInstance()->GetisLoaded();
	if (isLoaded == true)
	{
		InputManager::GetInstance()->Update();
		DebugManager::GetInstance()->Update();
		if (DebugManager::GetInstance()->GetisStop() == false ||
			DebugManager::GetInstance()->GetisNextFrame() == true)
		{
			LightManager::GetInstance()->Update();
			SceneManager::GetInstance()->Update();
			DebugManager::GetInstance()->SetisNextFrame(false);
		}
	}
}

void NewEngine::Draw()
{
	//　ロード終了チェック
	bool isLoaded = LoadManager::GetInstance()->GetisLoaded();
	if (isLoaded == true)
	{
		SceneManager::GetInstance()->DrawBackSprite();
		SceneManager::GetInstance()->DrawModel();
		ColliderDrawer::GetInstance()->DrawCollider();
		SceneManager::GetInstance()->DrawFrontSprite();
		SceneManager::GetInstance()->DrawRenderTexture();
		DebugManager::GetInstance()->DrawDebugGui();
	}
}

void NewEngine::PrevDraw()
{
	//　ロード終了チェック
	bool isLoaded = LoadManager::GetInstance()->GetisLoaded();
	if (isLoaded == true)
	{
		// SRVヒープの設定コマンド
		auto temp = TextureManager::GetSrvDescHeap();
		RenderBase::GetInstance()->GetCommandList()->SetDescriptorHeaps(1, &temp);
		SceneManager::GetInstance()->RenderTextureSetting();

		RenderBase::GetInstance()->PreDraw();
		Gui::PreDraw();
	}
}

void NewEngine::PostDraw()
{
	//　ロード終了チェック
	bool isLoaded = LoadManager::GetInstance()->GetisLoaded();
	if (isLoaded == true)
	{
		Gui::PostDraw();
		RenderBase::GetInstance()->PostDraw();
	}
}

void NewEngine::FrameControl()
{
	FrameRate::GetInstance()->Update();
}

bool NewEngine::ProcessMessage()
{
	//ウインドウズのメッセージを処理する
	if (RenderWindow::GetInstance()->ProcessMessage() == WM_QUIT)
	{
		return true;
	}
	return false;
}

Vec2 GetWindowSize()
{
	return RenderWindow::GetInstance()->GetWindowSize();
}
Vec2 GetWindowHalfSize()
{
	return RenderWindow::GetInstance()->GetWindowSize() / 2;
}
