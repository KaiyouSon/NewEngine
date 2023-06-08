#include "RenderWindow.h"
#include "imgui_impl_win32.h"
#include <Windows.h>
#pragma comment(lib,"winmm.lib")

void RenderWindow::CreateGameWindow()
{
	// �V�X�e���^�C�}�[�̕���\���グ��
	timeBeginPeriod(1);

	// �E�B���h�E�N���X�̐ݒ�
	wndClass_.cbSize = sizeof(WNDCLASSEX);
	wndClass_.lpfnWndProc = (WNDPROC)WindowProc;		// �E�C���h�E�v���V�[�W����ݒ�
	wndClass_.lpszClassName = L"DirectXGame";		// �E�B���h�E�N���X��
	wndClass_.hInstance = GetModuleHandle(nullptr);	// �E�B���h�E�n���h��
	wndClass_.hCursor = LoadCursor(NULL, IDC_ARROW);	// �J�[�\���w��

	// �E�B���h�E�N���X��OS�ɓo�^
	RegisterClassEx(&wndClass_);
	// �E�B���h�E�T�C�Y{ X���W Y���W �c�� ���� }
	RECT wrc = { 0,0,(LONG)size_.x,(LONG)size_.y };
	// �����ŃT�C�Y�␳����
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	std::wstring wTITLE(title_.begin(), title_.end());

	// �E�B���h�E�I�u�W�F�N�g�̐���
	hwnd_ = CreateWindow(
		wndClass_.lpszClassName, // �N���X��
		wTITLE.c_str(),			// �^�C�g���o�[�̕���
		WS_OVERLAPPEDWINDOW,	// �W���I�ȃE�B���h�E�X�^�C��
		wrc.left,				// �\��X���W(OS�ɔC����)
		0,						// �\��Y���W(OS�ɔC����)
		wrc.right - wrc.left,	// �E�B���h�E����
		wrc.bottom - wrc.top,	// �E�B���h�E�c��
		nullptr,				// �e�E�B���h�E�n���h��
		nullptr,				// ���j���[�n���h��
		wndClass_.hInstance,		// �Ăяo���A�v���P�[�V�����n���h��
		nullptr);				// �I�v�V����

	// �E�B���h�E��\����Ԃɂ���
	ShowWindow(hwnd_, SW_SHOW);
}

void RenderWindow::TerminateGameWindow()
{
	// �E�B���h�E�N���X��o�^����
	UnregisterClass(wndClass_.lpszClassName, wndClass_.hInstance);
}

unsigned int RenderWindow::ProcessMessage()
{
	// ���b�Z�[�W������?
	if (PeekMessage(&msg_, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg_);	// �L�[���̓��b�Z�[�W�̏���
		DispatchMessage(&msg_);		// �v���V�[�W���Ƀ��b�Z�[�W�𑗂�
	}

	return msg_.message;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
// �E�B���h�E�v���V�[�W��
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