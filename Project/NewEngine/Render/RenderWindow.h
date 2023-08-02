#pragma once
#include "Vec2.h"
#include "Util.h"
#include <Windows.h>
#include <string>

class RenderWindow : public Singleton<RenderWindow>
{
private:
	Vec2 mSize;			// �T�C�Y
	std::string mTitle;	// �^�C�g��

	WNDCLASSEX mWndClass;	// �E�B���h�E�N���X
	HWND mHwnd;			// �E�B���h�E�n���h��
	MSG mMsg;				//���b�Z�[�W

public:
	RenderWindow();

	// �Q�[���E�B���h�E�̍쐬 
	void CreateGameWindow();

	// �Q�[���E�B���h�E�̔j��
	void TerminateGameWindow();

	// �E�B���h�E���b�Z�[�W����
	uint32_t ProcessMessage();

public: // �Z�b�^�[
	void SetWindowTitle(const std::string& title);
	void SetWindowSize(const Vec2 size);

public: // �Q�b�^�[
	Vec2 GetWindowSize();
	HWND GetHwnd();
	HINSTANCE GetHInstance();

private:
	friend Singleton<RenderWindow>;
};

// �E�B���h�E�v���V�[�W��
LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);