#include "InputManager.h"
#include "KeyBoardInput.h"
#include "RenderWindow.h"
#include <cassert>
#include <memory>

#pragma region その他の処理

KeyBoardInput::KeyBoardInput()
{
}

void KeyBoardInput::Init()
{
	HRESULT result;
	RenderWindow* renderWindow = RenderWindow::GetInstance().get();

	// キーボードデバイスの生成
	result = InputManager::GetInstance()->GetDirectInput()->
		CreateDevice(GUID_SysKeyboard, &keyboard_, nullptr);
	assert(SUCCEEDED(result));

	// 入力データ形式のセット
	result = keyboard_->SetDataFormat(&c_dfDIKeyboard);	// 標準形式
	assert(SUCCEEDED(result));

	// 排他制御レベルのセット
	result = keyboard_->SetCooperativeLevel(
		renderWindow->GetHwnd(),
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}

void KeyBoardInput::Update()
{
	// キーボート情報の取得開始
	keyboard_->Acquire();

	// 最新のキーボード情報だったものは1フレーム前のキーボード情報として保存
	for (uint32_t i = 0; i < 256; ++i)
	{
		prevKeys_[i] = keys_[i];
	}

	// 最新のキーボード情報を取得する
	keyboard_->GetDeviceState(sizeof(keys_), keys_.data());
}

#pragma endregion

#pragma region キーボードの処理

// キーが押されてる時
bool KeyBoardInput::GetKey(const unsigned int key)
{
	// 例外スローしないように
	int index = Clamp<int>(key, 0, 256);

	return (bool)GetInstance()->keys_[index];
}

// キーを押した瞬間
bool KeyBoardInput::GetKeyDown(const unsigned int key)
{
	// 例外スローしないように
	int index = Clamp<int>(key, 0, 256);

	return (bool)(GetInstance()->keys_[key] && !GetInstance()->prevKeys_[key]);
}

// キーを離した瞬間
bool KeyBoardInput::GetKeyUp(const unsigned int key)
{
	// 例外スローしないように
	int index = Clamp<int>(key, 0, 256);

	return (bool)(!GetInstance()->keys_[key] && GetInstance()->prevKeys_[key]);
}

#pragma endregion
