#pragma once
#include "Vec2.h"
#include "Util.h"
#include <Windows.h>
#include <string>

class RenderWindow : public Singleton<RenderWindow>
{
private:
	Vec2 mSize;			// 繧ｵ繧､繧ｺ
	std::string mTitle;	// 繧ｿ繧､繝医Ν

	WNDCLASSEX mWndClass;	// 繧ｦ繧｣繝ｳ繝峨え繧ｯ繝ｩ繧ｹ
	HWND mHwnd;			// 繧ｦ繧｣繝ｳ繝峨え繝上Φ繝峨Ν
	MSG mMsg;				//繝｡繝・そ繝ｼ繧ｸ

public:
	RenderWindow();

	// 繧ｲ繝ｼ繝繧ｦ繧｣繝ｳ繝峨え縺ｮ菴懈・ 
	void CreateGameWindow();

	// 繧ｲ繝ｼ繝繧ｦ繧｣繝ｳ繝峨え縺ｮ遐ｴ譽・
	void TerminateGameWindow();

	// 繧ｦ繧｣繝ｳ繝峨え繝｡繝・そ繝ｼ繧ｸ蜃ｦ逅・
	uint32_t ProcessMessage();

public: // 繧ｻ繝・ち繝ｼ
	void SetWindowTitle(const std::string& title);
	void SetWindowSize(const Vec2 size);

public: // 繧ｲ繝・ち繝ｼ
	Vec2 GetWindowSize();
	HWND GetHwnd();
	HINSTANCE GetHInstance();

private:
	friend Singleton<RenderWindow>;
};

// 繧ｦ繧｣繝ｳ繝峨え繝励Ο繧ｷ繝ｼ繧ｸ繝｣
LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
