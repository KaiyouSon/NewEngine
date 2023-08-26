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
	RenderWindow::GetInstance()->TerminateGameWindow();		// �E�B���h�E�N���X��o�^����
	SoundManager::Destroy();

	RenderBase::Destroy();
}

void NewEngine::Setting()
{
	// �E�B���h�E�^�C�g��
	mRenderWindow->SetWindowTitle(mSetting.windowTitle);

	// �E�B���h�E�T�C�Y
	mRenderWindow->SetWindowSize(mSetting.windowSize);

	// �w�i�F
	mRenderBase->sClearColor[0] = mSetting.bgColor.To01().r;
	mRenderBase->sClearColor[1] = mSetting.bgColor.To01().g;
	mRenderBase->sClearColor[2] = mSetting.bgColor.To01().b;
}

void NewEngine::Init()
{
	Setting();

	// �E�B���h�E����
	mRenderWindow->CreateGameWindow();

	// RenderBase�̏�����
	mRenderBase->Init();

	// �����h�̏�����
	Random::Init();

	//GUI�̏�����
	Gui::Init();

	// �T�E���h�}�l�[�W���̏�����
	SoundManager::Init();

	// �t���[�����[�g�̏�����
	FrameRate::GetInstance()->Init(mSetting.frameRate);

	// �C���v�b�g�}�l�[�W���̏�����
	InputManager::GetInstance()->Init();

	// �f�o�b�O�}�l�[�W���[�̏�����
	DebugManager::GetInstance()->Init();

	// ���[�h�}�l�[�W���̃��[�h
	LoadManager::GetInstance()->Load();

	//�@���[�h�I���`�F�b�N
	bool isLoaded = LoadManager::GetInstance()->GetisLoaded();
	if (isLoaded == true)
	{
		// �R���C�_�[�h�����[�̃��[�h�Ə�����
		ColliderDrawer::GetInstance()->Load();
		ColliderDrawer::GetInstance()->Init();

		// �V�[���}�l�[�W���[�̏�����
		SceneManager::GetInstance()->Init();
	}
}

void NewEngine::Update()
{
	//�@���[�h�I���`�F�b�N
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
	//�@���[�h�I���`�F�b�N
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
	//�@���[�h�I���`�F�b�N
	bool isLoaded = LoadManager::GetInstance()->GetisLoaded();
	if (isLoaded == true)
	{
		// SRV�q�[�v�̐ݒ�R�}���h
		auto temp = TextureManager::GetSrvDescHeap();
		RenderBase::GetInstance()->GetCommandList()->SetDescriptorHeaps(1, &temp);
		SceneManager::GetInstance()->RenderTextureSetting();

		RenderBase::GetInstance()->PreDraw();
		Gui::PreDraw();
	}
}

void NewEngine::PostDraw()
{
	//�@���[�h�I���`�F�b�N
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
	//�E�C���h�E�Y�̃��b�Z�[�W����������
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
