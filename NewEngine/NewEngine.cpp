#include "NewEngine.h"
#include "GuiManager.h"
#include "RenderBase.h"
#include "RenderWindow.h"
#include "DeveloperManager.h"
#include "DataOperator.h"
#include "RenderTexture.h"
#include "NewEngine/main2.h"
#include <wrl.h>
using namespace Microsoft::WRL;

void NewEngineInit()
{
	RenderBase* renderBase = RenderBase::GetInstance().get();
	RenderWindow* renderWindow = RenderWindow::GetInstance().get();
	GuiManager* guiManager = GuiManager::GetInstance().get();

#ifdef _DEBUG
	//デバッグレイヤーをオンに
	ComPtr<ID3D12Debug1> debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(debugController.GetAddressOf()))))
	{
		debugController->EnableDebugLayer();
		debugController->SetEnableGPUBasedValidation(TRUE);
	}
#endif

#ifdef NDEBUG
	dataOperator->Initialize();
	dataOperator->LoadWindowData();

	renderWindow->SetWindowTitle(
		dataOperator->GetGameWindowTitleForStorage());
	renderWindow->SetWindowSize(
		{
			dataOperator->GetGameWindowSizeForStorage().x,
			dataOperator->GetGameWindowSizeForStorage().y
		});
#endif

#ifdef _DEBUG
	renderWindow->SetWindowTitle("NewEngine");
	renderWindow->SetWindowSize({ 1920, 1010 });
#endif

	renderWindow->CreateGameWindow();

	renderBase->Initialize();

#ifdef _DEBUG
	ComPtr<ID3D12InfoQueue> infoQuene;
	if (SUCCEEDED(renderBase->GetDevice()->QueryInterface(IID_PPV_ARGS(&infoQuene))))
	{
		infoQuene->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);	// やばいエラー一時に止まる
		infoQuene->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);		// エラー時に止まる
		infoQuene->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);	// ワーニング時に止まる
	}
#endif

	developerManager->Initialize();

#ifdef _DEBUG
	guiManager->Initialize();
#endif

	sceneViewTexture->Initialize({ 960,540 });
}
void NewEngineUpda()
{
	developerManager->Update();

#ifdef _DEBUG
	GuiManager::GetInstance()->Update();
#endif
}
void NewEnginePreDraw()
{
#ifdef _DEBUG
	sceneViewTexture->PreDrawScene();
#endif
}
void NewEneineDraw()
{
#ifdef _DEBUG
	developerManager->Draw();
#endif
}
void NewEnginePostDraw()
{
#ifdef _DEBUG
	sceneViewTexture->PostDrawScene();
#endif
	RenderBase* renderBase = RenderBase::GetInstance().get();

	renderBase->PreDraw();

#ifdef _DEBUG
	GuiManager::GetInstance()->Draw();
#endif

#ifdef NDEBUG
	DeveloperManager::GetInstance()->Draw();
#endif
	renderBase->GetInstance()->PostDraw();


}
void NewEngineEnd()
{
	// ウィンドウクラスを登録解除
	RenderWindow::GetInstance()->TerminateGameWindow();

	//ComPtr<ID3D12Device> tmpDevice = RenderBase::GetInstance()->GetDevice().Get();
#ifdef _DEBUG
	GuiManager::GetInstance()->Destroy();
#endif

	//ID3D12DebugDevice* debugInterface;
	//if (SUCCEEDED(tmpDevice.Get()->QueryInterface(&debugInterface)))
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
void SetBackGroundColor(float Red, float Green, float Blue)
{
	RenderBase::GetInstance()->clearColor[0] = Red / 255;
	RenderBase::GetInstance()->clearColor[1] = Green / 255;
	RenderBase::GetInstance()->clearColor[2] = Blue / 255;
}
