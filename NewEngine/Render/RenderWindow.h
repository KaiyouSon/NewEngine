#pragma once
#include "Vec2.h"
#include "Util.h"
#include <Windows.h>
#include <string>

class RenderWindow : public Singleton<RenderWindow>
{
private:
	friend Singleton<RenderWindow>;
	Vec2 size = { 1280 ,720 };		// サイズ
	std::string title = "error";	// タイトル

	WNDCLASSEX wndClass;	// ウィンドウクラス
	HWND hwnd;				// ウィンドウハンドル
	MSG msg;				//メッセージ
public:
	// ウィンドウプロシージャ


public:
	// ゲームウィンドウの作成 
	void CreateGameWindow();

	// ゲームウィンドウの破棄
	void TerminateGameWindow();

	// ウィンドウメッセージ処理
	unsigned int ProcessMessage();

public: // セッター
	inline void SetWindowTitle(const std::string& title) { this->title = title; }
	inline void SetWindowSize(const Vec2 size) { this->size = size; }

public: // ゲッター
	inline Vec2 GetWindowSize() { return size; }

	// ウィンドウハンドルの取得
	inline HWND GetHwnd() { return hwnd; }
	inline HINSTANCE GetHInstance() { return wndClass.hInstance; }
};

LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);