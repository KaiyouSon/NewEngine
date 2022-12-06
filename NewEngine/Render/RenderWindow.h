#pragma once
#include "Vec2.h"
#include "Util.h"
#include <Windows.h>
#include <string>

class RenderWindow : public Singleton<RenderWindow>
{
private:
	friend Singleton<RenderWindow>;
	Vec2 size = { 1280 ,720 };		// �T�C�Y
	std::string title = "error";	// �^�C�g��

	WNDCLASSEX wndClass = {};	// �E�B���h�E�N���X
	HWND hwnd = {};				// �E�B���h�E�n���h��
	MSG msg = {};				//���b�Z�[�W
public:
	// �E�B���h�E�v���V�[�W��


public:
	// �Q�[���E�B���h�E�̍쐬 
	void CreateGameWindow();

	// �Q�[���E�B���h�E�̔j��
	void TerminateGameWindow();

	// �E�B���h�E���b�Z�[�W����
	unsigned int ProcessMessage();

public: // �Z�b�^�[
	inline void SetWindowTitle(const std::string& title) { this->title = title; }
	inline void SetWindowSize(const Vec2 size) { this->size = size; }

public: // �Q�b�^�[
	inline Vec2 GetWindowSize() { return size; }

	// �E�B���h�E�n���h���̎擾
	inline HWND GetHwnd() { return hwnd; }
	inline HINSTANCE GetHInstance() { return wndClass.hInstance; }
};

LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);