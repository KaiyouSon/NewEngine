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
	RenderWindow::GetInstance()->TerminateGameWindow();		// 繧ｦ繧｣繝ｳ繝峨え繧ｯ繝ｩ繧ｹ繧堤匳骭ｲ隗｣髯､
	SoundManager::Destroy();

	RenderBase::Destroy();
}

void NewEngine::Setting()
{
	// 繧ｦ繧｣繝ｳ繝峨え繧ｿ繧､繝医Ν
	mRenderWindow->SetWindowTitle(mSetting.windowTitle);

	// 繧ｦ繧｣繝ｳ繝峨え繧ｵ繧､繧ｺ
	mRenderWindow->SetWindowSize(mSetting.windowSize);

	// 閭梧勹濶ｲ
	mRenderBase->sClearColor[0] = mSetting.bgColor.To01().r;
	mRenderBase->sClearColor[1] = mSetting.bgColor.To01().g;
	mRenderBase->sClearColor[2] = mSetting.bgColor.To01().b;
}

void NewEngine::Init()
{
	Setting();

	// 繧ｦ繧｣繝ｳ繝峨え逕滓・
	mRenderWindow->CreateGameWindow();

	// RenderBase縺ｮ蛻晄悄蛹・
	mRenderBase->Init();

	// 繝ｩ繝ｳ繝峨・蛻晄悄蛹・
	Random::Init();

	//GUI縺ｮ蛻晄悄蛹・
	Gui::Init();

	// 繧ｵ繧ｦ繝ｳ繝峨・繝阪・繧ｸ繝｣縺ｮ蛻晄悄蛹・
	SoundManager::Init();

	// 繝輔Ξ繝ｼ繝繝ｬ繝ｼ繝医・蛻晄悄蛹・
	FrameRate::GetInstance()->Init(mSetting.frameRate);

	// 繧､繝ｳ繝励ャ繝医・繝阪・繧ｸ繝｣縺ｮ蛻晄悄蛹・
	InputManager::GetInstance()->Init();

	// 繝・ヰ繝・げ繝槭ロ繝ｼ繧ｸ繝｣繝ｼ縺ｮ蛻晄悄蛹・
	DebugManager::GetInstance()->Init();

	// 繝ｭ繝ｼ繝峨・繝阪・繧ｸ繝｣縺ｮ繝ｭ繝ｼ繝・
	LoadManager::GetInstance()->Load();

	//縲繝ｭ繝ｼ繝臥ｵゆｺ・メ繧ｧ繝・け
	bool isLoaded = LoadManager::GetInstance()->GetisLoaded();
	if (isLoaded == true)
	{
		// 繧ｳ繝ｩ繧､繝繝ｼ繝峨Ο繝ｯ繝ｼ縺ｮ繝ｭ繝ｼ繝峨→蛻晄悄蛹・
		ColliderDrawer::GetInstance()->Load();
		ColliderDrawer::GetInstance()->Init();

		// 繧ｷ繝ｼ繝ｳ繝槭ロ繝ｼ繧ｸ繝｣繝ｼ縺ｮ蛻晄悄蛹・
		SceneManager::GetInstance()->Init();
	}
}

void NewEngine::Update()
{
	//縲繝ｭ繝ｼ繝臥ｵゆｺ・メ繧ｧ繝・け
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
	//縲繝ｭ繝ｼ繝臥ｵゆｺ・メ繧ｧ繝・け
	bool isLoaded = LoadManager::GetInstance()->GetisLoaded();
	if (isLoaded == true)
	{
		SceneManager::GetInstance()->Draw();
		ColliderDrawer::GetInstance()->DrawCollider();
		SceneManager::GetInstance()->DrawDebugGui();
		DebugManager::GetInstance()->DrawDebugGui();
	}
}

void NewEngine::PrevDraw()
{
	//縲繝ｭ繝ｼ繝臥ｵゆｺ・メ繧ｧ繝・け
	bool isLoaded = LoadManager::GetInstance()->GetisLoaded();
	if (isLoaded == true)
	{
		// SRV繝偵・繝励・險ｭ螳壹さ繝槭Φ繝・
		auto srvDescHeap = DescriptorHeapManager::GetDescriptorHeap("SRV")->GetDescriptorHeap();
		RenderBase::GetInstance()->GetCommandList()->SetDescriptorHeaps(1, &srvDescHeap);
		SceneManager::GetInstance()->RenderTextureSetting();

		RenderBase::GetInstance()->PreDraw();
		Gui::PreDraw();
	}
}

void NewEngine::PostDraw()
{
	//縲繝ｭ繝ｼ繝臥ｵゆｺ・メ繧ｧ繝・け
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
	//繧ｦ繧､繝ｳ繝峨え繧ｺ縺ｮ繝｡繝・そ繝ｼ繧ｸ繧貞・逅・☆繧・
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

