#pragma once
#include "Vec2.h"
#include "Util.h"
#include <Windows.h>
#include <string>

class RenderWindow : public Singleton<RenderWindow>
{
private:
	Vec2 size_ = { 1920 ,1080 };	// サイズ
	std::string title_ = "error";	// タイトル

	WNDCLASSEX wndClass_ = {};	// ウィンドウクラス
	HWND hwnd_ = {};				// ウィンドウハンドル
	MSG msg_ = {};				//メッセージ

public:
	// ゲームウィンドウの作成 
	void CreateGameWindow();

	// ゲームウィンドウの破棄
	void TerminateGameWindow();

	// ウィンドウメッセージ処理
	unsigned int ProcessMessage();

public: // セッター
	inline void SetWindowTitle(const std::string& title) { this->title_ = title; }
	inline void SetWindowSize(const Vec2 size) { this->size_ = size; }

public: // ゲッター
	inline Vec2 GetWindowSize() { return size_; }

	// ウィンドウハンドルの取得
	inline HWND GetHwnd() { return hwnd_; }
	inline HINSTANCE GetHInstance() { return wndClass_.hInstance; }

private:
	friend Singleton<RenderWindow>;
	RenderWindow() {};
};

// ウィンドウプロシージャ
LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);