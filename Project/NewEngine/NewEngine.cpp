#include "NewEngine.h"
#include "RenderBase.h"
#include "RenderWindow.h"
#include "RenderTexture.h"
#include "LoadManager.h"
#include "DebugManager.h"
#include <wrl.h>
using namespace Microsoft::WRL;

void NewEngineInit()
{
	ProcessAtDebugBulid([]()
		{
			//デバッグレイヤーをオンに
			ComPtr<ID3D12Debug1> debugController;
			if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(debugController.GetAddressOf()))))
			{
				debugController->EnableDebugLayer();
				//debugController->SetEnableGPUBasedValidation(true);
			}
		});

	RenderWindow::GetInstance()->CreateGameWindow();
	RenderBase::GetInstance()->Init();

	ProcessAtDebugBulid([]()
		{
			ComPtr<ID3D12InfoQueue> infoQueue;
			if (SUCCEEDED(RenderBase::GetInstance()->
				GetDevice()->QueryInterface(IID_PPV_ARGS(&infoQueue))))
			{
				infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);	// やばいエラー一時に止まる
				infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);		// エラー時に止まる
				infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);	// ワーニング時に止まる
			}

			//抑制するエラー
			D3D12_MESSAGE_ID denyIds[] = {
				D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE
			};

			//抑制される表示レベル
			D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };
			D3D12_INFO_QUEUE_FILTER filter{};
			filter.DenyList.NumIDs = _countof(denyIds);
			filter.DenyList.pIDList = denyIds;
			filter.DenyList.NumSeverities = _countof(severities);
			filter.DenyList.pSeverityList = severities;
			//指定したエラーの表示を抑制する
			infoQueue->PushStorageFilter(&filter);
		});

	// -------------------------------------------------------------------------------- //

	Random::Init();
	Gui::Init();
	SoundManager::Init();
	InputManager::GetInstance()->Init();
	DebugManager::GetInstance()->Init();
	LoadManager::GetInstance()->Load();

	//　ロード終了チェック
	bool isLoaded = LoadManager::GetInstance()->GetisLoaded();
	if (isLoaded == true)
	{
		ColliderDrawer::GetInstance()->Load();
		ColliderDrawer::GetInstance()->Init();
		SceneManager::GetInstance()->Init();
	}

	// -------------------------------------------------------------------------------- //

}
void NewEngineUpda()
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
void NewEnginePreDraw()
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
void NewEneineDraw()
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
void NewEnginePostDraw()
{
	//　ロード終了チェック
	bool isLoaded = LoadManager::GetInstance()->GetisLoaded();
	if (isLoaded == true)
	{
		Gui::PostDraw();
		RenderBase::GetInstance()->PostDraw();
	}
}
void NewEngineEnd()
{
	Gui::Destroy();
	RenderWindow::GetInstance()->TerminateGameWindow();		// ウィンドウクラスを登録解除
	SoundManager::Destroy();

	RenderBase::Destroy();
}

bool ProcessMessage()
{
	//ウインドウズのメッセージを処理する
	if (RenderWindow::GetInstance()->ProcessMessage() == WM_QUIT)
	{
		return true;
	}
	return false;
}
void SetWindowTitle(const std::string& title)
{
	RenderWindow::GetInstance()->SetWindowTitle(title);
}
void SetWindowSize(const Vec2& size)
{
	RenderWindow::GetInstance()->SetWindowSize(size);
}
void SetBackGroundColor(const float& r, const float& g, const float& b)
{
	RenderBase::sClearColor[0] = r / 255;
	RenderBase::sClearColor[1] = g / 255;
	RenderBase::sClearColor[2] = b / 255;
}
Vec2 GetWindowSize()
{
	return RenderWindow::GetInstance()->GetWindowSize();
}
Vec2 GetWindowHalfSize()
{
	return RenderWindow::GetInstance()->GetWindowSize() / 2;
}

void SetFrameRate(const float& frameRate)
{
	FrameRate::GetInstance()->Init(frameRate);
}
void FrameRateUpdate()
{
	FrameRate::GetInstance()->Update();
}
