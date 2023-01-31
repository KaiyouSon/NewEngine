#include "NewEngine.h"
#include "RenderBase.h"
#include "RenderWindow.h"
#include "RenderTexture.h"
#include <wrl.h>
using namespace Microsoft::WRL;

void NewEngineInit()
{
#ifdef _DEBUG 
	//デバッグレイヤーをオンに
	ComPtr<ID3D12Debug1> debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(debugController.GetAddressOf()))))
	{
		debugController->EnableDebugLayer();
		debugController->SetEnableGPUBasedValidation(TRUE);
	}
#endif
	// -------------------------------------------------------------------------------- //

	RenderWindow::GetInstance()->CreateGameWindow();
	RenderBase::GetInstance()->Init();
	Sound::Init();
	Random::Init();
	InputManager::GetInstance()->Init();
	GuiManager::GetInstance()->Init();
	//DirectionalLight::current.Init();
	SceneManager::GetInstance()->Init();

	// -------------------------------------------------------------------------------- //
#ifdef _DEBUG
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
#endif
}
void NewEngineUpda()
{
	InputManager::GetInstance()->Update();
	SceneManager::GetInstance()->Update();
	//DirectionalLight::current.Update();
}
void NewEnginePreDraw()
{
	SceneManager::GetInstance()->DrawRenderTexture();

	RenderBase::GetInstance()->PreDraw();
	GuiManager::GetInstance()->PreDraw();
}
void NewEneineDraw()
{
	RenderBase::GetInstance()->SetSpriteDrawCommand();
	SceneManager::GetInstance()->DrawBackSprite();

	RenderBase::GetInstance()->SetObject3DDrawCommand();
	SceneManager::GetInstance()->DrawModel();

	RenderBase::GetInstance()->SetSpriteDrawCommand();
	SceneManager::GetInstance()->DrawFrontSprite();
}
void NewEnginePostDraw()
{
	GuiManager::GetInstance()->PostDraw();
	RenderBase::GetInstance()->PostDraw();
}
void NewEngineEnd()
{
	GuiManager::GetInstance()->Destroy();
	// ウィンドウクラスを登録解除
	RenderWindow::GetInstance()->TerminateGameWindow();

	//ComPtr<ID3D12Device> tempDevice = RenderBase::GetInstance()->GetDevice();
	RenderBase::Destroy();

	//ID3D12DebugDevice* debugInterface;
	//if (SUCCEEDED(tempDevice->QueryInterface(&debugInterface)))
	//{
	//	debugInterface->ReportLiveDeviceObjects(D3D12_RLDO_DETAIL | D3D12_RLDO_IGNORE_INTERNAL);
	//	debugInterface->Release();
	//}
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
	RenderBase::clearColor[0] = r / 255;
	RenderBase::clearColor[1] = g / 255;
	RenderBase::clearColor[2] = b / 255;
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
