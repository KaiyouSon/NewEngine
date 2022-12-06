#include "RenderWindow.h"
#include "imgui_impl_win32.h"
#include <Windows.h>
#pragma comment(lib,"winmm.lib")

void RenderWindow::CreateGameWindow()
{
	// システムタイマーの分解能を上げる
	timeBeginPeriod(1);

	// ウィンドウクラスの設定
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.lpfnWndProc = (WNDPROC)WindowProc;		// ウインドウプロシージャを設定
	wndClass.lpszClassName = L"DirectXGame";		// ウィンドウクラス名
	wndClass.hInstance = GetModuleHandle(nullptr);	// ウィンドウハンドル
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);	// カーソル指定

	// ウィンドウクラスをOSに登録
	RegisterClassEx(&wndClass);
	// ウィンドウサイズ{ X座標 Y座標 縦幅 横幅 }
	RECT wrc = { 0,0,(LONG)size.x,(LONG)size.y };
	// 自動でサイズ補正する
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	std::wstring wTITLE(title.begin(), title.end());

	// ウィンドウオブジェクトの生成
	hwnd = CreateWindow(
		wndClass.lpszClassName, // クラス名
		wTITLE.c_str(),			// タイトルバーの文字
		WS_OVERLAPPEDWINDOW,	// 標準的なウィンドウスタイル
		wrc.left,				// 表示X座標(OSに任せる)
		0,						// 表示Y座標(OSに任せる)
		wrc.right - wrc.left,	// ウィンドウ横幅
		wrc.bottom - wrc.top,	// ウィンドウ縦幅
		nullptr,				// 親ウィンドウハンドル
		nullptr,				// メニューハンドル
		wndClass.hInstance,		// 呼び出しアプリケーションハンドル
		nullptr);				// オプション

	// ウィンドウを表示状態にする
	ShowWindow(hwnd, SW_SHOW);
}

void RenderWindow::TerminateGameWindow()
{
	// ウィンドウクラスを登録解除
	UnregisterClass(wndClass.lpszClassName, wndClass.hInstance);
}

unsigned int RenderWindow::ProcessMessage()
{
	// メッセージがある?
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);	// キー入力メッセージの処理
		DispatchMessage(&msg);	// プロシージャにメッセージを送る
	}

	return msg.message;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
// ウィンドウプロシージャ
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