#pragma once
#include "Vec2.h"
#include "Util.h"
#include <Windows.h>
#include <string>

class RenderWindow : public Singleton<RenderWindow>
{
private:
	Vec2 size_ = { 1920 ,1080 };	// �T�C�Y
	std::string title_ = "error";	// �^�C�g��

	WNDCLASSEX wndClass_ = {};	// �E�B���h�E�N���X
	HWND hwnd_ = {};				// �E�B���h�E�n���h��
	MSG msg_ = {};				//���b�Z�[�W

public:
	// �Q�[���E�B���h�E�̍쐬 
	void CreateGameWindow();

	// �Q�[���E�B���h�E�̔j��
	void TerminateGameWindow();

	// �E�B���h�E���b�Z�[�W����
	unsigned int ProcessMessage();

public: // �Z�b�^�[
	inline void SetWindowTitle(const std::string& title) { this->title_ = title; }
	inline void SetWindowSize(const Vec2 size) { this->size_ = size; }

public: // �Q�b�^�[
	inline Vec2 GetWindowSize() { return size_; }

	// �E�B���h�E�n���h���̎擾
	inline HWND GetHwnd() { return hwnd_; }
	inline HINSTANCE GetHInstance() { return wndClass_.hInstance; }

private:
	friend Singleton<RenderWindow>;
	RenderWindow() {};
};

// �E�B���h�E�v���V�[�W��
LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);