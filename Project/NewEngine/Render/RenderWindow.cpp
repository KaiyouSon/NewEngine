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
	// システムタイマーの分解能を上げる
	timeBeginPeriod(1);

	// ウィンドウクラスの設定
	mWndClass.cbSize = sizeof(WNDCLASSEX);
	mWndClass.lpfnWndProc = (WNDPROC)WindowProc;		// ウインドウプロシージャを設定
	mWndClass.lpszClassName = L"DirectXGame";		// ウィンドウクラス名
	mWndClass.hInstance = GetModuleHandle(nullptr);	// ウィンドウハンドル
	mWndClass.hCursor = LoadCursor(NULL, IDC_ARROW);	// カーソル指定

	// ウィンドウクラスをOSに登録
	RegisterClassEx(&mWndClass);
	// ウィンドウサイズ{ X座標 Y座標 縦幅 横幅 }
	RECT wrc = { 0,0,(LONG)mSize.x,(LONG)mSize.y };
	// 自動でサイズ補正する
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	std::wstring wTtile(mTitle.begin(), mTitle.end());

	// ウィンドウオブジェクトの生成
	mHwnd = (HWND)CreateWindow(
		mWndClass.lpszClassName, // クラス名
		wTtile.c_str(),			 // タイトルバーの文字
		WS_OVERLAPPEDWINDOW,	 // 標準的なウィンドウスタイル
		wrc.left,				 // 表示X座標(OSに任せる)
		0,						 // 表示Y座標(OSに任せる)
		wrc.right - wrc.left,	 // ウィンドウ横幅
		wrc.bottom - wrc.top,	 // ウィンドウ縦幅
		nullptr,				 // 親ウィンドウハンドル
		nullptr,				 // メニューハンドル
		mWndClass.hInstance,	 // 呼び出しアプリケーションハンドル
		nullptr);				 // オプション

	// ウィンドウを表示状態にする
	ShowWindow(mHwnd, SW_SHOW);
}

void RenderWindow::TerminateGameWindow()
{
	// ウィンドウクラスを登録解除
	UnregisterClass(mWndClass.lpszClassName, mWndClass.hInstance);
}

unsigned int RenderWindow::ProcessMessage()
{
	// メッセージがある?
	if (PeekMessage(&mMsg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&mMsg);	// キー入力メッセージの処理
		DispatchMessage(&mMsg);		// プロシージャにメッセージを送る
	}

	return mMsg.message;
}

// セッター
void RenderWindow::SetWindowTitle(const std::string& title) { mTitle = title; }
void RenderWindow::SetWindowSize(const Vec2 size) { mSize = size; }

// ゲッター
Vec2 RenderWindow::GetWindowSize() { return mSize; }
HWND RenderWindow::GetHwnd() { return mHwnd; }
HINSTANCE RenderWindow::GetHInstance() { return mWndClass.hInstance; }

// ウィンドウプロシージャ
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
	{
		return true;
	}

	// メッセージに応じてゲーム固有の処理を行う
	switch (msg)
	{
	case WM_SIZE:
		break;
	case WM_DESTROY:			// ウィンドウが破壊された
		// OSに対して、アプリの終了を伝える
		PostQuitMessage(0);
		return 0;
	}

	// 標準のメッセージ処理を行う
	return DefWindowProc(hwnd, msg, wparam, lparam);
}