#include "InputManager.h"
#include "MouseInput.h"
#include "RenderWindow.h"
#include <cassert>

MouseInput::MouseInput() :
	mMouse(nullptr), mMouseInput({}), mPrevMouseInput({})
{
}

void MouseInput::Init()
{
	HRESULT result;
	RenderWindow* renderWindow = RenderWindow::GetInstance().get();

	// マウスデバイスの取得
	result = InputManager::GetInstance()->GetDirectInput()->
		CreateDevice(GUID_SysMouse, &mMouse, nullptr);
	assert(SUCCEEDED(result));

	// マウスデータフォーマットの設定
	result = mMouse->SetDataFormat(&c_dfDIMouse2); // マウス情報の設定
	assert(SUCCEEDED(result));

	// 協調モードの設定
	result = mMouse->SetCooperativeLevel(
		renderWindow->GetHwnd(),
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	assert(SUCCEEDED(result));
}

void MouseInput::Update()
{
	RenderWindow* renderWindow = RenderWindow::GetInstance().get();

	// マウスデバイスの取得
	mMouse->Acquire();

	// 現在のマウス入力情報を一時保存
	mPrevMouseInput = mMouseInput;

	// 現在のマウス入力情報を取得
	mMouse->GetDeviceState(sizeof(mMouseInput), &mMouseInput);

	POINT tempMousePos;
	GetCursorPos(&tempMousePos);

	// スクリーン座標をウィンドウ座標に変換
	ScreenToClient(renderWindow->GetHwnd(), &tempMousePos);
	mMousePos.x = static_cast<float>(tempMousePos.x);
	mMousePos.y = static_cast<float>(tempMousePos.y);
}

// マウスボタンが押されているかを取得
bool MouseInput::GetClick(const MouseCode mouseCode)
{
	bool isClick = (GetInstance()->mMouseInput.rgbButtons[(int)mouseCode] & 0x80) != 0;

	return isClick;
}

// マウスボタンがクリックされた瞬間かを取得
bool MouseInput::GetClickDown(const MouseCode mouseCode)
{
	bool isClick = (GetInstance()->mMouseInput.rgbButtons[(int)mouseCode] & 0x80) != 0;
	bool isPrevClick = (GetInstance()->mPrevMouseInput.rgbButtons[(int)mouseCode] & 0x80) != 0;

	return isClick && !isPrevClick;
}

// マウスボタンが離された瞬間かを取得
bool MouseInput::GetClickUp(const MouseCode mouseCode)
{
	bool isClick = (GetInstance()->mMouseInput.rgbButtons[(int)mouseCode] & 0x80) != 0;
	bool isPrevClick = (GetInstance()->mPrevMouseInput.rgbButtons[(int)mouseCode] & 0x80) != 0;

	return !isClick && isPrevClick;
}

// マウスの位置を取得
Vec2 MouseInput::GetPos()
{
	return GetInstance()->mMousePos;
}

// マウスの移動ベクトルを取得
Vec2 MouseInput::GetMoveVec()
{
	return { (float)GetInstance()->mMouseInput.lX, (float)GetInstance()->mMouseInput.lY };
}

// マウスホイールの移動量を取得
float MouseInput::GetWheelMoveVec()
{
	return (float)GetInstance()->mMouseInput.lZ;
}
