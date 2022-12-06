#include "NewEngine.h"
#include "RenderBase.h"
#include "RenderWindow.h"
#include "RenderTexture.h"
#include <wrl.h>
using namespace Microsoft::WRL;

void NewEngineInit()
{
#ifdef _DEBUG 
	//�f�o�b�O���C���[���I����
	ComPtr<ID3D12Debug1> debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(debugController.GetAddressOf()))))
	{
		debugController->EnableDebugLayer();
		debugController->SetEnableGPUBasedValidation(TRUE);
	}
#endif
	// -------------------------------------------------------------------------------- //

	RenderBase* renderBase = RenderBase::GetInstance().get();
	RenderWindow* renderWindow = RenderWindow::GetInstance().get();

	renderWindow->CreateGameWindow();
	renderBase->Initialize();
	Sound::Init();
	InputManager::GetInstance()->Initialize();
	SceneManager::GetInstance()->Init();
	GuiManager::GetInstance()->Init();

	// -------------------------------------------------------------------------------- //
#ifdef _DEBUG
	ComPtr<ID3D12InfoQueue> infoQuene;
	if (SUCCEEDED(renderBase->GetDevice()->QueryInterface(IID_PPV_ARGS(&infoQuene))))
	{
		infoQuene->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);	// ��΂��G���[�ꎞ�Ɏ~�܂�
		infoQuene->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);		// �G���[���Ɏ~�܂�
		infoQuene->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);	// ���[�j���O���Ɏ~�܂�
	}
#endif
}
void NewEngineUpda()
{
	InputManager::GetInstance()->Update();
	SceneManager::GetInstance()->Update();
}
void NewEnginePreDraw()
{
	RenderBase::GetInstance()->PreDraw();
}
void NewEneineDraw()
{
	GuiManager::GetInstance()->PreDraw();
	SceneManager::GetInstance()->Draw();
	GuiManager::GetInstance()->PostDraw();
}
void NewEnginePostDraw()
{
	RenderBase::GetInstance()->PostDraw();
}
void NewEngineEnd()
{
	GuiManager::GetInstance()->Destroy();
	// �E�B���h�E�N���X��o�^����
	RenderWindow::GetInstance()->TerminateGameWindow();
	//ComPtr<ID3D12Device> tempDevice = RenderBase::GetInstance()->GetDevice().Get();
	//ID3D12DebugDevice* debugInterface;
	//if (SUCCEEDED(tmpDevice.Get()->QueryInterface(&debugInterface)))
	//{
	//	debugInterface->ReportLiveDeviceObjects(D3D12_RLDO_DETAIL | D3D12_RLDO_IGNORE_INTERNAL);
	//	debugInterface->Release();
	//}
}

bool ProcessMessage()
{
	//�E�C���h�E�Y�̃��b�Z�[�W����������
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
	RenderBase::GetInstance()->clearColor[0] = r / 255;
	RenderBase::GetInstance()->clearColor[1] = g / 255;
	RenderBase::GetInstance()->clearColor[2] = b / 255;
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
	FrameRate::GetInstance()->Initialize(frameRate);
}
void FrameRateUpdate()
{
	FrameRate::GetInstance()->Update();
}
