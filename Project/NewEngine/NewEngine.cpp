#include "NewEngine.h"
#include "RenderBase.h"
#include "RenderWindow.h"
#include "RenderTexture.h"
#include "LoadManager.h"
#include <wrl.h>
using namespace Microsoft::WRL;

void NewEngineInit()
{
	ProcessAtDebugBulid([]()
		{
			//�f�o�b�O���C���[���I����
			ComPtr<ID3D12Debug1> debugController;
			if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(debugController.GetAddressOf()))))
			{
				debugController->EnableDebugLayer();
				debugController->SetEnableGPUBasedValidation(true);
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
				infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);	// ��΂��G���[�ꎞ�Ɏ~�܂�
				infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);		// �G���[���Ɏ~�܂�
				infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);	// ���[�j���O���Ɏ~�܂�
			}

			//�}������G���[
			D3D12_MESSAGE_ID denyIds[] = {
				D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE
			};

			//�}�������\�����x��
			D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };
			D3D12_INFO_QUEUE_FILTER filter{};
			filter.DenyList.NumIDs = _countof(denyIds);
			filter.DenyList.pIDList = denyIds;
			filter.DenyList.NumSeverities = _countof(severities);
			filter.DenyList.pSeverityList = severities;
			//�w�肵���G���[�̕\����}������
			infoQueue->PushStorageFilter(&filter);
		});

	// -------------------------------------------------------------------------------- //

	Random::Init();
	SoundManager::Init();
	InputManager::GetInstance()->Init();
	GuiManager::GetInstance()->Init();
	LoadManager::GetInstance()->Load();

	//�@���[�h�I���`�F�b�N
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
	//�@���[�h�I���`�F�b�N
	bool isLoaded = LoadManager::GetInstance()->GetisLoaded();
	if (isLoaded == true)
	{
		InputManager::GetInstance()->Update();
		LightManager::GetInstance()->Update();
		SceneManager::GetInstance()->Update();
	}
}
void NewEnginePreDraw()
{
	//�@���[�h�I���`�F�b�N
	bool isLoaded = LoadManager::GetInstance()->GetisLoaded();
	if (isLoaded == true)
	{
		// SRV�q�[�v�̐ݒ�R�}���h
		auto temp = TextureManager::GetSrvDescHeap();
		RenderBase::GetInstance()->GetCommandList()->SetDescriptorHeaps(1, &temp);
		SceneManager::GetInstance()->RenderTextureSetting();

		RenderBase::GetInstance()->PreDraw();
		GuiManager::GetInstance()->PreDraw();
	}
}
void NewEneineDraw()
{
	//�@���[�h�I���`�F�b�N
	bool isLoaded = LoadManager::GetInstance()->GetisLoaded();
	if (isLoaded == true)
	{
		SceneManager::GetInstance()->DrawBackSprite();
		SceneManager::GetInstance()->DrawModel();
		ColliderDrawer::GetInstance()->DrawCollider();
		SceneManager::GetInstance()->DrawFrontSprite();
		SceneManager::GetInstance()->DrawRenderTexture();
	}
}
void NewEnginePostDraw()
{
	//�@���[�h�I���`�F�b�N
	bool isLoaded = LoadManager::GetInstance()->GetisLoaded();
	if (isLoaded == true)
	{
		GuiManager::GetInstance()->PostDraw();
		RenderBase::GetInstance()->PostDraw();
	}
}
void NewEngineEnd()
{
	GuiManager::GetInstance()->Destroy();
	RenderWindow::GetInstance()->TerminateGameWindow();		// �E�B���h�E�N���X��o�^����
	SoundManager::Destroy();

	RenderBase::Destroy();
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
