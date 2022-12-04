#include "InputManager.h"
#include "JoypadInput.h"
#include "RenderWindow.h"
#include <cassert>
#pragma comment(lib, "xinput.lib")

void JoypadInput::Initialize()
{
	HRESULT result;
	RenderWindow* renderWindow = RenderWindow::GetInstance().get();

	// ジョイパットデバイスの列挙
	result = InputManager::GetInstance()->GetDirectInput()->
		EnumDevices(
			DI8DEVCLASS_GAMECTRL,
			DeviceFindCallBack,
			this,
			DIEDFL_ATTACHEDONLY);
	assert(SUCCEEDED(result));

	if (joypad == nullptr) return;

	// 入力データ形式のセット
	result = joypad->SetDataFormat(&c_dfDIJoystick2);
	assert(SUCCEEDED(result));

	// 排他制御レベルのセット
	result = joypad->SetCooperativeLevel(
		renderWindow->GetHwnd(),
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	assert(SUCCEEDED(result));

	// 軸のモードを絶対軸に設定
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(DIPROPDWORD);
	diprop.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.diph.dwObj = 0;
	diprop.dwData = DIPROPAXISMODE_ABS;
	joypad->SetProperty(DIPROP_AXISMODE, &diprop.diph);

	//軸の当たりの範囲を設定
	DIPROPRANGE diprg;
	diprg.diph.dwSize = sizeof(DIPROPRANGE);
	diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	diprg.diph.dwHow = DIPH_BYOFFSET;
	diprg.lMin = -32768;
	diprg.lMax = 32767;
	diprg.diph.dwObj = DIJOFS_X;
	joypad->SetProperty(DIPROP_RANGE, &diprg.diph);
	diprg.diph.dwObj = DIJOFS_Y;
	joypad->SetProperty(DIPROP_RANGE, &diprg.diph);
}

void JoypadInput::Update()
{
	if (joypad == nullptr) return;

	// ジョイパット情報の取得開始
	joypad->Acquire();

	// 最新のパット情報だったものは1フレーム前のキーボード情報として保存
	oldPadInput = padInput;

	// 最新のジョイパット情報を取得する
	joypad->GetDeviceState(sizeof(padInput), &padInput);
}

bool JoypadInput::GetButton(const int& buttonNumber)
{
	return padInput.rgbButtons[buttonNumber] & 0x80;
}

bool JoypadInput::GetButtonTrigger(const int& buttonNumber)
{
	return (padInput.rgbButtons[buttonNumber] & 0x80) &&
		!(oldPadInput.rgbButtons[buttonNumber] & 0x80);
}

bool JoypadInput::GetButtonReleased(const int& buttonNumber)
{
	return !(padInput.rgbButtons[buttonNumber] & 0x80) &&
		(oldPadInput.rgbButtons[buttonNumber] & 0x80);
}

Vec2 JoypadInput::GetLeftStickParam()
{
	return Vec2(padInput.lX, padInput.lY);
}

Vec2 JoypadInput::GetLeftStickVec()
{
	return Vec2(padInput.lX, padInput.lY).Normalized();
}

Vec2 JoypadInput::GetRightStickParam()
{
	return Vec2(padInput.lRx, padInput.lRy);
}

Vec2 JoypadInput::GetRightStickVec()
{
	return Vec2(padInput.lRx, padInput.lRy).Normalized();
}

// デバイス発見時に実行される
BOOL CALLBACK JoypadInput::DeviceFindCallBack(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext)
{
	HRESULT result;
	//Input* input = static_cast<Input*>(pContext);
	JoypadInput* joypadInput = static_cast<JoypadInput*>(pContext);
	Microsoft::WRL::ComPtr<IDirectInputDevice8>  joypad;
	result = InputManager::GetInstance()->GetDirectInput()->
		CreateDevice(pdidInstance->guidInstance, &joypad, nullptr);
	if (FAILED(result)) return DIENUM_CONTINUE;

	DIDEVICEINSTANCE instance;
	joypad->GetDeviceInfo(&instance);
	joypadInput->joypad = joypad;

	return DIENUM_CONTINUE;
}