#pragma once
#include "Vec2.h"
#include "Util.h"
#include <Windows.h>
#include <string>

class RenderWindow : public Singleton<RenderWindow>
{
private:
	Vec2 mSize;			// サイズ
	std::string mTitle;	// タイトル

	WNDCLASSEX mWndClass;	// ウィンドウクラス
	HWND mHwnd;			// ウィンドウハンドル
	MSG mMsg;				//メッセージ

public:
	RenderWindow();

	// ゲームウィンドウの作成 
	void CreateGameWindow();

	// ゲームウィンドウの破棄
	void TerminateGameWindow();

	// ウィンドウメッセージ処理
	uint32_t ProcessMessage();

public: // セッター
	void SetWindowTitle(const std::string& title);
	void SetWindowSize(const Vec2 size);

public: // ゲッター
	Vec2 GetWindowSize();
	HWND GetHwnd();
	HINSTANCE GetHInstance();

private:
	friend Singleton<RenderWindow>;
};

// ウィンドウプロシージャ
LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);