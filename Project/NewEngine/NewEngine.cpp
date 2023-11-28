#include "NewEngine.h"
#include "LoadManager.h"
#include "DebugManager.h"
using namespace Microsoft::WRL;

bool NewEngine::sIsClose = false;

NewEngine::NewEngine(const NewEngineSetting& setting) :
	mSetting(setting),
	mRenderWindow(RenderWindow::GetInstance().get()),
	mRenderBase(RenderBase::GetInstance())
{
}

NewEngine::~NewEngine()
{
	// GUIの破棄
	Gui::Destroy();

	// ウィンドウクラスを登録解除
	RenderWindow::GetInstance()->TerminateGameWindow();

	// サウンドの破棄
	SoundManager::Destroy();

	// RenderBaseの破棄
	RenderBase::Destroy();
}

void NewEngine::Setting()
{
	// ウィンドウタイトルの設定
	mRenderWindow->SetWindowTitle(mSetting.windowTitle);

	// ウィンドウサイズの設定
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

	// ランダムの初期化
	Random::Init();

	// GUIの初期化
	Gui::Init();

	// サウンドの初期化
	SoundManager::Init();

	// フレームレートの初期化
	FrameRate::GetInstance()->Init(mSetting.frameRate);

	// 入力の初期化
	InputManager::GetInstance()->Init();

	// デバッグ機能の初期化
	DebugManager::GetInstance()->Init();

	// ロード
	LoadManager::GetInstance()->Load();

	// ロード終了なら
	bool isLoaded = LoadManager::GetInstance()->GetisLoaded();
	if (isLoaded == true)
	{
		// コライダードロワーのロードと初期化
		ColliderDrawer::GetInstance()->Load();
		ColliderDrawer::GetInstance()->Init();

		// シーンの初期化
		SceneManager::GetInstance()->Init();
	}
}

void NewEngine::Update()
{
	mIsLoaded = LoadManager::GetInstance()->GetisLoaded();

	// ロード終わってないなら
	if (mIsLoaded == false)
	{
		return;
	}

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

void NewEngine::Draw()
{
	// ロード終わってないなら
	if (mIsLoaded == false)
	{
		return;
	}

	SceneManager::GetInstance()->Draw();
	Renderer::GetInstance()->DrawObject();

	ColliderDrawer::GetInstance()->DrawCollider();
	SceneManager::GetInstance()->DrawDebugGui();
	DebugManager::GetInstance()->DrawDebugGui();
}

void NewEngine::PrevDraw()
{
	// ロード終わってないなら
	if (mIsLoaded == false)
	{
		return;
	}

	// SRVのヒープセット
	auto srvDescHeap = DescriptorHeapManager::GetDescriptorHeap("SRV")->GetDescriptorHeap();
	RenderBase::GetInstance()->GetCommandList()->SetDescriptorHeaps(1, &srvDescHeap);

	// メインに描画する前にCSの実行とパスの描画を先に
	SceneManager::GetInstance()->ExecuteCS();
	SceneManager::GetInstance()->DrawPass();

	RenderBase::GetInstance()->PreDraw();
	Gui::PreDraw();
}

void NewEngine::PostDraw()
{
	// ロード終わってないなら
	if (mIsLoaded == false)
	{
		return;
	}

	Gui::PostDraw();
	RenderBase::GetInstance()->PostDraw();
}

void NewEngine::FrameControl()
{
	FrameRate::GetInstance()->Update();
}

bool NewEngine::ProcessMessage()
{
	// ウィンドウメッセージを処理し、WM_QUIT が返された場合は true を返す
	if (RenderWindow::GetInstance()->ProcessMessage() == WM_QUIT)
	{
		return true;
	}
	return false;
}

bool NewEngine::GetisClose()
{
	return sIsClose;
}

void NewEngine::SetisClose(const bool isClose)
{
	sIsClose = isClose;
}

Vec2 GetWindowSize()
{
	return RenderWindow::GetInstance()->GetWindowSize();
}
Vec2 GetWindowHalfSize()
{
	return RenderWindow::GetInstance()->GetWindowSize() / 2;
}

