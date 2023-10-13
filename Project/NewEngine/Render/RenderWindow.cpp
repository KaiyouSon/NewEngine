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
	// 繧ｷ繧ｹ繝・Β繧ｿ繧､繝槭・縺ｮ蛻・ｧ｣閭ｽ繧剃ｸ翫￡繧・
	timeBeginPeriod(1);

	// 繧ｦ繧｣繝ｳ繝峨え繧ｯ繝ｩ繧ｹ縺ｮ險ｭ螳・
	mWndClass.cbSize = sizeof(WNDCLASSEX);
	mWndClass.lpfnWndProc = (WNDPROC)WindowProc;		// 繧ｦ繧､繝ｳ繝峨え繝励Ο繧ｷ繝ｼ繧ｸ繝｣繧定ｨｭ螳・
	mWndClass.lpszClassName = L"DirectXGame";		// 繧ｦ繧｣繝ｳ繝峨え繧ｯ繝ｩ繧ｹ蜷・
	mWndClass.hInstance = GetModuleHandle(nullptr);	// 繧ｦ繧｣繝ｳ繝峨え繝上Φ繝峨Ν
	mWndClass.hCursor = LoadCursor(NULL, IDC_ARROW);	// 繧ｫ繝ｼ繧ｽ繝ｫ謖・ｮ・

	// 繧ｦ繧｣繝ｳ繝峨え繧ｯ繝ｩ繧ｹ繧丹S縺ｫ逋ｻ骭ｲ
	RegisterClassEx(&mWndClass);
	// 繧ｦ繧｣繝ｳ繝峨え繧ｵ繧､繧ｺ{ X蠎ｧ讓・Y蠎ｧ讓・邵ｦ蟷・讓ｪ蟷・}
	RECT wrc = { 0,0,(LONG)mSize.x,(LONG)mSize.y };
	// 閾ｪ蜍輔〒繧ｵ繧､繧ｺ陬懈ｭ｣縺吶ｋ
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	std::wstring wTtile(mTitle.begin(), mTitle.end());

	// 繧ｦ繧｣繝ｳ繝峨え繧ｪ繝悶ず繧ｧ繧ｯ繝医・逕滓・
	mHwnd = (HWND)CreateWindow(
		mWndClass.lpszClassName, // 繧ｯ繝ｩ繧ｹ蜷・
		wTtile.c_str(),			 // 繧ｿ繧､繝医Ν繝舌・縺ｮ譁・ｭ・
		WS_OVERLAPPEDWINDOW,	 // 讓呎ｺ也噪縺ｪ繧ｦ繧｣繝ｳ繝峨え繧ｹ繧ｿ繧､繝ｫ
		wrc.left,				 // 陦ｨ遉ｺX蠎ｧ讓・OS縺ｫ莉ｻ縺帙ｋ)
		0,						 // 陦ｨ遉ｺY蠎ｧ讓・OS縺ｫ莉ｻ縺帙ｋ)
		wrc.right - wrc.left,	 // 繧ｦ繧｣繝ｳ繝峨え讓ｪ蟷・
		wrc.bottom - wrc.top,	 // 繧ｦ繧｣繝ｳ繝峨え邵ｦ蟷・
		nullptr,				 // 隕ｪ繧ｦ繧｣繝ｳ繝峨え繝上Φ繝峨Ν
		nullptr,				 // 繝｡繝九Η繝ｼ繝上Φ繝峨Ν
		mWndClass.hInstance,	 // 蜻ｼ縺ｳ蜃ｺ縺励い繝励Μ繧ｱ繝ｼ繧ｷ繝ｧ繝ｳ繝上Φ繝峨Ν
		nullptr);				 // 繧ｪ繝励す繝ｧ繝ｳ

	// 繧ｦ繧｣繝ｳ繝峨え繧定｡ｨ遉ｺ迥ｶ諷九↓縺吶ｋ
	ShowWindow(mHwnd, SW_SHOW);
}

void RenderWindow::TerminateGameWindow()
{
	// 繧ｦ繧｣繝ｳ繝峨え繧ｯ繝ｩ繧ｹ繧堤匳骭ｲ隗｣髯､
	UnregisterClass(mWndClass.lpszClassName, mWndClass.hInstance);
}

unsigned int RenderWindow::ProcessMessage()
{
	// 繝｡繝・そ繝ｼ繧ｸ縺後≠繧・
	if (PeekMessage(&mMsg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&mMsg);	// 繧ｭ繝ｼ蜈･蜉帙Γ繝・そ繝ｼ繧ｸ縺ｮ蜃ｦ逅・
		DispatchMessage(&mMsg);		// 繝励Ο繧ｷ繝ｼ繧ｸ繝｣縺ｫ繝｡繝・そ繝ｼ繧ｸ繧帝√ｋ
	}

	return mMsg.message;
}

// 繧ｻ繝・ち繝ｼ
void RenderWindow::SetWindowTitle(const std::string& title) { mTitle = title; }
void RenderWindow::SetWindowSize(const Vec2 size) { mSize = size; }

// 繧ｲ繝・ち繝ｼ
Vec2 RenderWindow::GetWindowSize() { return mSize; }
HWND RenderWindow::GetHwnd() { return mHwnd; }
HINSTANCE RenderWindow::GetHInstance() { return mWndClass.hInstance; }

// 繧ｦ繧｣繝ｳ繝峨え繝励Ο繧ｷ繝ｼ繧ｸ繝｣
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
	{
		return true;
	}

	// 繝｡繝・そ繝ｼ繧ｸ縺ｫ蠢懊§縺ｦ繧ｲ繝ｼ繝蝗ｺ譛峨・蜃ｦ逅・ｒ陦後≧
	switch (msg)
	{
	case WM_SIZE:
		break;
	case WM_DESTROY:			// 繧ｦ繧｣繝ｳ繝峨え縺檎ｴ螢翫＆繧後◆
		// OS縺ｫ蟇ｾ縺励※縲√い繝励Μ縺ｮ邨ゆｺ・ｒ莨昴∴繧・
		PostQuitMessage(0);
		return 0;
	}

	// 讓呎ｺ悶・繝｡繝・そ繝ｼ繧ｸ蜃ｦ逅・ｒ陦後≧
	return DefWindowProc(hwnd, msg, wparam, lparam);
}
