#include "RenderWindow.h"
#include "imgui_impl_win32.h"
#include "AssetsWindow.h"
#include "StandardLib.h"

#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"shlwapi.lib") 

RenderWindow::RenderWindow() :
	mSize(Vec2(1920, 1080)), mTitle("empty")
{
}

// ゲームウィンドウの作成
void RenderWindow::CreateGameWindow()
{
	// タイムスライス精度を設定
	timeBeginPeriod(1);

	// ウィンドウクラスの設定
	mWndClass.cbSize = sizeof(WNDCLASSEX);
	mWndClass.lpfnWndProc = (WNDPROC)WindowProc;  // ウィンドウプロシージャの指定
	mWndClass.lpszClassName = L"NewEngine";      // ウィンドウクラス名
	mWndClass.hInstance = GetModuleHandle(nullptr);  // インスタンスハンドル
	mWndClass.hCursor = LoadCursor(NULL, IDC_ARROW);  // カーソル設定
	mWndClass.style = CS_OWNDC; // ドラッグ&ドロップを可能にするためにCS_OWNDCスタイルを追加

	// ウィンドウクラスの登録
	RegisterClassEx(&mWndClass);

	// ウィンドウのクライアント領域サイズを設定
	RECT wrc = { 0, 0, (LONG)mSize.x, (LONG)mSize.y };
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	std::wstring wTitle(mTitle.begin(), mTitle.end());

	// ゲームウィンドウの作成
	mHwnd = (HWND)CreateWindow(
		mWndClass.lpszClassName,  // ウィンドウクラス名
		wTitle.c_str(),           // ウィンドウタイトル
		WS_OVERLAPPEDWINDOW,      // ウィンドウスタイル
		wrc.left,                 // ウィンドウのX座標
		0,                        // ウィンドウのY座標
		wrc.right - wrc.left,     // ウィンドウの幅
		wrc.bottom - wrc.top,     // ウィンドウの高さ
		nullptr,                  // 親ウィンドウハンドル
		nullptr,                  // メニューハンドル
		mWndClass.hInstance,      // インスタンスハンドル
		nullptr);                 // 追加のアプリケーションデータ

	// ゲームウィンドウを表示
	ShowWindow(mHwnd, SW_SHOW);

	// ファイルのドロップを許可
	DragAcceptFiles(mHwnd, true);
}

// ゲームウィンドウの終了
void RenderWindow::TerminateGameWindow()
{
	// ウィンドウクラスの登録解除
	UnregisterClass(mWndClass.lpszClassName, mWndClass.hInstance);
}

// メッセージ処理関数
uint32_t RenderWindow::ProcessMessage()
{
	// メッセージキューからメッセージを取得
	if (PeekMessage(&mMsg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&mMsg);  // キー入力メッセージの変換
		DispatchMessage(&mMsg);   // メッセージ処理関数にメッセージを送信
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

// ImGui関連のウィンドウプロシージャハンドラ
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// ウィンドウプロシージャコールバック
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
	{
		return true;
	}

	// 通常のウィンドウ処理を行い、特定のメッセージに応じて追加の処理を行う
	switch (msg)
	{
	case WM_SIZE:
		// ウィンドウのサイズ変更が発生した場合の処理を追加
		break;
	case WM_DESTROY:
		// ウィンドウが破棄される前の処理を追加
		// OSにプログラム終了を通知し、メッセージキューからプログラムを終了する
		PostQuitMessage(0);
		return 0;

	case WM_DROPFILES:
		// ドロップの処理

		if (!AssetsWindow::GetMouseInWindow())
		{
			return 0;
		}

		HDROP hDrop = (HDROP)wParam;
		UINT numFiles = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);
		for (UINT i = 0; i < numFiles; ++i)
		{
			WCHAR szFilePath[MAX_PATH];
			DragQueryFile(hDrop, i, szFilePath, MAX_PATH);
			// ここでファイルのパスを使用して処理を行う

			// ファイルパスから拡張子を取得
			WCHAR* ext = PathFindExtension(szFilePath);
			if (!ext)
			{
				continue;
			}

			// pngファイルであれば
			if (lstrcmpi(ext, L".png") == 0)
			{
				OutputDebugLog("PngFile");

				// プロジェクトの指定フォルダにコピー
				std::wstring wNewPath;
				CopyFileToDestination(szFilePath, WAppTextureDirectory.c_str(), &wNewPath);

				// テクスチャをロードする
				std::string path = WStrToStr(wNewPath);
				LoadTexture(path);
			}
		}

		DragFinish(hDrop);
		return 0;
	}

	// 通常のウィンドウプロシージャに処理を委譲
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
