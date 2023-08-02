#include "RenderWindow.h"
#include "imgui_impl_win32.h"
#include <Windows.h>
#pragma comment(lib,"winmm.lib")

RenderWindow::RenderWindow() :
	mSize(Vec2(1920, 1080)), mTitle("empty")
{
}

void RenderWindow::CreateGameWindow()
{
	// �V�X�e���^�C�}�[�̕���\���グ��
	timeBeginPeriod(1);

	// �E�B���h�E�N���X�̐ݒ�
	mWndClass.cbSize = sizeof(WNDCLASSEX);
	mWndClass.lpfnWndProc = (WNDPROC)WindowProc;		// �E�C���h�E�v���V�[�W����ݒ�
	mWndClass.lpszClassName = L"DirectXGame";		// �E�B���h�E�N���X��
	mWndClass.hInstance = GetModuleHandle(nullptr);	// �E�B���h�E�n���h��
	mWndClass.hCursor = LoadCursor(NULL, IDC_ARROW);	// �J�[�\���w��

	// �E�B���h�E�N���X��OS�ɓo�^
	RegisterClassEx(&mWndClass);
	// �E�B���h�E�T�C�Y{ X���W Y���W �c�� ���� }
	RECT wrc = { 0,0,(LONG)mSize.x,(LONG)mSize.y };
	// �����ŃT�C�Y�␳����
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	std::wstring wTtile(mTitle.begin(), mTitle.end());

	// �E�B���h�E�I�u�W�F�N�g�̐���
	mHwnd = (HWND)CreateWindow(
		mWndClass.lpszClassName, // �N���X��
		wTtile.c_str(),			 // �^�C�g���o�[�̕���
		WS_OVERLAPPEDWINDOW,	 // �W���I�ȃE�B���h�E�X�^�C��
		wrc.left,				 // �\��X���W(OS�ɔC����)
		0,						 // �\��Y���W(OS�ɔC����)
		wrc.right - wrc.left,	 // �E�B���h�E����
		wrc.bottom - wrc.top,	 // �E�B���h�E�c��
		nullptr,				 // �e�E�B���h�E�n���h��
		nullptr,				 // ���j���[�n���h��
		mWndClass.hInstance,	 // �Ăяo���A�v���P�[�V�����n���h��
		nullptr);				 // �I�v�V����

	// �E�B���h�E��\����Ԃɂ���
	ShowWindow(mHwnd, SW_SHOW);
}

void RenderWindow::TerminateGameWindow()
{
	// �E�B���h�E�N���X��o�^����
	UnregisterClass(mWndClass.lpszClassName, mWndClass.hInstance);
}

unsigned int RenderWindow::ProcessMessage()
{
	// ���b�Z�[�W������?
	if (PeekMessage(&mMsg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&mMsg);	// �L�[���̓��b�Z�[�W�̏���
		DispatchMessage(&mMsg);		// �v���V�[�W���Ƀ��b�Z�[�W�𑗂�
	}

	return mMsg.message;
}

// �Z�b�^�[
void RenderWindow::SetWindowTitle(const std::string& title) { mTitle = title; }
void RenderWindow::SetWindowSize(const Vec2 size) { mSize = size; }

// �Q�b�^�[
Vec2 RenderWindow::GetWindowSize() { return mSize; }
HWND RenderWindow::GetHwnd() { return mHwnd; }
HINSTANCE RenderWindow::GetHInstance() { return mWndClass.hInstance; }

// �E�B���h�E�v���V�[�W��
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
	{
		return true;
	}

	// ���b�Z�[�W�ɉ����ăQ�[���ŗL�̏������s��
	switch (msg)
	{
	case WM_SIZE:
		break;
	case WM_DESTROY:			// �E�B���h�E���j�󂳂ꂽ
		// OS�ɑ΂��āA�A�v���̏I����`����
		PostQuitMessage(0);
		return 0;
	}

	// �W���̃��b�Z�[�W�������s��
	return DefWindowProc(hwnd, msg, wparam, lparam);
}