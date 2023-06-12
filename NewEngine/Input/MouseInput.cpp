#include "InputManager.h"
#include "MouseInput.h"
#include "RenderWindow.h"
#include <cassert>

#pragma region その他の処理

MouseInput::MouseInput() :
	mouse_(nullptr), mouseInput_({}), prevMouseInput_({})
{
}

void MouseInput::Init()
{
	HRESULT result;
	RenderWindow* renderWindow = RenderWindow::GetInstance().get();

	// マウスデバイスの生成
	result = InputManager::GetInstance()->GetDirectInput()->
		CreateDevice(GUID_SysMouse, &mouse_, nullptr);
	assert(SUCCEEDED(result));

	// 入力データ形式のセット
	result = mouse_->SetDataFormat(&c_dfDIMouse2); // 拡張8ボタンまで
	assert(SUCCEEDED(result));

	// 排他制御レベルのセット
	result = mouse_->SetCooperativeLevel(
		renderWindow->GetHwnd(),
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	assert(SUCCEEDED(result));
}

void MouseInput::Update()
{
	RenderWindow* renderWindow = RenderWindow::GetInstance().get();

	// マウス情報の取得開始
	mouse_->Acquire();

	// 最新のマウス情報だったものは1フレーム前のキーボード情報として保存
	prevMouseInput_ = mouseInput_;

	// 最新のマウス情報を取得する
	mouse_->GetDeviceState(sizeof(mouseInput_), &mouseInput_);

	POINT tempMousePos;
	GetCursorPos(&tempMousePos);

	// ウィンドウ座標に変換する
	ScreenToClient(renderWindow->GetHwnd(), &tempMousePos);
	mousePos_.x = static_cast<float>(tempMousePos.x);
	mousePos_.y = static_cast<float>(tempMousePos.y);
}

#pragma endregion

#pragma region マウスの処理

// クリックしている間
bool MouseInput::GetClick(const MouseCodo mouseCodo)
{
	bool isClick = (GetInstance()->mouseInput_.rgbButtons[(int)mouseCodo] & 0x80) != 0;

	return isClick;
}

// クリックした瞬間
bool MouseInput::GetClickDown(const MouseCodo mouseCodo)
{
	bool isClick = (GetInstance()->mouseInput_.rgbButtons[(int)mouseCodo] & 0x80) != 0;
	bool isPrevClick = (GetInstance()->prevMouseInput_.rgbButtons[(int)mouseCodo] & 0x80) != 0;

	return isClick && !isPrevClick;
}

// クリックし終わった瞬間
bool MouseInput::GetClickUp(const MouseCodo mouseCodo)
{
	bool isClick = (GetInstance()->mouseInput_.rgbButtons[(int)mouseCodo] & 0x80) != 0;
	bool isPrevClick = (GetInstance()->prevMouseInput_.rgbButtons[(int)mouseCodo] & 0x80) != 0;

	return !isClick && isPrevClick;
}

// マウスの座標
Vec2 MouseInput::GetPos()
{
	return GetInstance()->mousePos_;
}

// マウスの動いているベクトル
Vec2 MouseInput::GetMoveVec()
{
	return { (float)GetInstance()->mouseInput_.lX, (float)GetInstance()->mouseInput_.lY };
}

// マウスホイルの動いているベクトル
float MouseInput::GetWheelMoveVec()
{
	return (float)GetInstance()->mouseInput_.lZ;
}

#pragma endregion