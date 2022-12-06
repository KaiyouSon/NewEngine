#include "InputManager.h"
#include "KeyBoardInput.h"
#include "RenderWindow.h"
#include <cassert>
#include <memory>

void KeyBoardInput::Init()
{
	HRESULT result;
	RenderWindow* renderWindow = RenderWindow::GetInstance().get();

	// キーボードデバイスの生成
	result = InputManager::GetInstance()->GetDirectInput()->
		CreateDevice(GUID_SysKeyboard, &keyboard, nullptr);
	assert(SUCCEEDED(result));

	// 入力データ形式のセット
	result = keyboard->SetDataFormat(&c_dfDIKeyboard);	// 標準形式
	assert(SUCCEEDED(result));

	// 排他制御レベルのセット
	result = keyboard->SetCooperativeLevel(
		renderWindow->GetHwnd(),
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}

void KeyBoardInput::Update()
{
	// キーボート情報の取得開始
	keyboard->Acquire();

	// 最新のキーボード情報だったものは1フレーム前のキーボード情報として保存
	for (uint32_t i = 0; i < 256; ++i)
	{
		prevKeys[i] = keys[i];
	}

	// 最新のキーボード情報を取得する
	keyboard->GetDeviceState(sizeof(keys), keys);
}
