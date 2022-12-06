#include "InputManager.h"
#include "MouseInput.h"
#include "RenderWindow.h"
#include <cassert>

MouseInput::MouseInput() : mouse(nullptr), mouseInput({}), prevMouseInput({})
{
}

void MouseInput::Init()
{
	HRESULT result;
	RenderWindow* renderWindow = RenderWindow::GetInstance().get();

	// マウスデバイスの生成
	result = InputManager::GetInstance()->GetDirectInput()->
		CreateDevice(GUID_SysMouse, &mouse, nullptr);
	assert(SUCCEEDED(result));

	// 入力データ形式のセット
	result = mouse->SetDataFormat(&c_dfDIMouse2); // 拡張8ボタンまで
	assert(SUCCEEDED(result));

	// 排他制御レベルのセット
	result = mouse->SetCooperativeLevel(
		renderWindow->GetHwnd(),
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	assert(SUCCEEDED(result));
}

void MouseInput::Update()
{
	RenderWindow* renderWindow = RenderWindow::GetInstance().get();

	// マウス情報の取得開始
	mouse->Acquire();

	// 最新のマウス情報だったものは1フレーム前のキーボード情報として保存
	prevMouseInput = mouseInput;

	// 最新のマウス情報を取得する
	mouse->GetDeviceState(sizeof(mouseInput), &mouseInput);

	POINT tempMousePos;
	GetCursorPos(&tempMousePos);

	// ウィンドウ座標に変換する
	ScreenToClient(renderWindow->GetHwnd(), &tempMousePos);
	mousePos.x = static_cast<float>(tempMousePos.x);
	mousePos.y = static_cast<float>(tempMousePos.y);
}
