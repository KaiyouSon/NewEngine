#include "InputManager.h"
#include "JoypadInput.h"
#include "RenderWindow.h"
#include <cassert>
#include <dbt.h>

bool JoypadInput::sIsInsertPad = false;
uint32_t JoypadInput::sPadIndex = 0;

// メッセージ処理用のコールバック関数
LRESULT SubWindowProc(int code, WPARAM wParam, LPARAM lParam)
{
	if (code < 0)
	{
		return CallNextHookEx(NULL, code, wParam, lParam);
	}

	PCWPSTRUCT msg = PCWPSTRUCT(lParam);
	if (msg->message == WM_DEVICECHANGE)
	{
		switch (msg->wParam)
		{
		case DBT_DEVICEARRIVAL:
		case DBT_DEVICEREMOVECOMPLETE:
			Pad::GetInstance()->SetisInsertPad(true);
			break;
		}
	}

	return CallNextHookEx(NULL, code, wParam, lParam);
}

JoypadInput::JoypadInput()
{
}
void JoypadInput::Init()
{
	SetJoyStick();

	// デバイスの接続/切断通知
	DEV_BROADCAST_DEVICEINTERFACE notificationFilter{};
	notificationFilter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
	notificationFilter.dbcc_size = sizeof(notificationFilter);

	HDEVNOTIFY notifyResult = RegisterDeviceNotification(
		RenderWindow::GetInstance()->GetHwnd(), &notificationFilter,
		DEVICE_NOTIFY_WINDOW_HANDLE | DEVICE_NOTIFY_ALL_INTERFACE_CLASSES);
	notifyResult;
	assert(!!notifyResult);

	SetWindowsHookExW(
		WH_CALLWNDPROC, (HOOKPROC)&SubWindowProc, GetModuleHandleW(NULL), GetCurrentThreadId());
}
void JoypadInput::Update()
{
	if (sIsInsertPad == true)
	{
		mJoypadObjs.clear();
		SetJoyStick();
		sIsInsertPad = false;
	}

	for (int i = 0; i < mJoypadObjs.size(); i++)
	{
		if (mJoypadObjs[i].joypad == nullptr) continue;

		// デバイス状態を取得
		mJoypadObjs[i].joypad->Acquire();

		// デバイスポーリング
		mJoypadObjs[i].joypad->Poll();

		// 前回のデバイス状態を保存
		mJoypadObjs[i].prevPadInput = mJoypadObjs[i].padInput;

		// デバイスの入力状態を取得
		mJoypadObjs[i].joypad->GetDeviceState(sizeof(mJoypadObjs[i].padInput), &mJoypadObjs[i].padInput);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// --- ボタン関連 ---------------------------------------------------------------------------------- ///
/////////////////////////////////////////////////////////////////////////////////////////////////////////

// ボタンの押下状態を取得
bool JoypadInput::GetButton(const PadCode padCode, const int padIndex)
{
	// リンクされているか確認
	if (GetisLinkPad() == false) return false;

	if (padCode == PadCode::ButtonLeft)
	{
		return GetInstance()->mJoypadObjs[padIndex].padInput.rgdwPOV[0] == 27000;
	}
	else if (padCode == PadCode::ButtonRight)
	{
		return GetInstance()->mJoypadObjs[padIndex].padInput.rgdwPOV[0] == 9000;
	}
	else if (padCode == PadCode::ButtonUp)
	{
		return GetInstance()->mJoypadObjs[padIndex].padInput.rgdwPOV[0] == 0;
	}
	else if (padCode == PadCode::ButtonDown)
	{
		return GetInstance()->mJoypadObjs[padIndex].padInput.rgdwPOV[0] == 18000;
	}

	return GetInstance()->mJoypadObjs[padIndex].padInput.rgbButtons[(uint32_t)padCode] & 0x80;
}

// ボタンが押された瞬間の状態を取得
bool JoypadInput::GetButtonDown(const PadCode padCode, const int padIndex)
{
	// リンクされているか確認
	if (GetisLinkPad() == false) return false;

	if (padCode == PadCode::ButtonLeft)
	{
		return (GetInstance()->mJoypadObjs[padIndex].padInput.rgdwPOV[0] == 27000) &&
			!(GetInstance()->mJoypadObjs[padIndex].prevPadInput.rgdwPOV[0] == 27000);
	}
	else if (padCode == PadCode::ButtonRight)
	{
		return GetInstance()->mJoypadObjs[padIndex].padInput.rgdwPOV[0] == 9000 &&
			!(GetInstance()->mJoypadObjs[padIndex].prevPadInput.rgdwPOV[0] == 9000);
	}
	else if (padCode == PadCode::ButtonUp)
	{
		return GetInstance()->mJoypadObjs[padIndex].padInput.rgdwPOV[0] == 0 &&
			!(GetInstance()->mJoypadObjs[padIndex].prevPadInput.rgdwPOV[0] == 0);
	}
	else if (padCode == PadCode::ButtonDown)
	{
		return GetInstance()->mJoypadObjs[padIndex].padInput.rgdwPOV[0] == 18000 &&
			!(GetInstance()->mJoypadObjs[padIndex].prevPadInput.rgdwPOV[0] == 18000);
	}

	if (padCode == PadCode::RightStick)
	{
		return (GetInstance()->mJoypadObjs[padIndex].padInput.rgbButtons[9] & 0x80) &&
			!(GetInstance()->mJoypadObjs[padIndex].prevPadInput.rgbButtons[9] & 0x80);
	}

	return (GetInstance()->mJoypadObjs[padIndex].padInput.rgbButtons[(uint32_t)padCode] & 0x80) &&
		!(GetInstance()->mJoypadObjs[padIndex].prevPadInput.rgbButtons[(uint32_t)padCode] & 0x80);
}

// ボタンが離された瞬間の状態を取得
bool JoypadInput::GetButtonUp(const PadCode padCode, const int padIndex)
{
	if (padCode == PadCode::ButtonLeft)
	{
		bool isPressed = GetInstance()->mJoypadObjs[padIndex].padInput.rgdwPOV[0] == 27000;
		bool wasPressed = GetInstance()->mJoypadObjs[padIndex].prevPadInput.rgdwPOV[0] == 27000;
		return !isPressed && wasPressed;
	}
	else if (padCode == PadCode::ButtonRight)
	{
		bool isPressed = GetInstance()->mJoypadObjs[padIndex].padInput.rgdwPOV[0] == 9000;
		bool wasPressed = GetInstance()->mJoypadObjs[padIndex].prevPadInput.rgdwPOV[0] == 9000;
		return !isPressed && wasPressed;
	}
	else if (padCode == PadCode::ButtonUp)
	{
		bool isPressed = GetInstance()->mJoypadObjs[padIndex].padInput.rgdwPOV[0] == 0;
		bool wasPressed = GetInstance()->mJoypadObjs[padIndex].prevPadInput.rgdwPOV[0] == 0;
		return !isPressed && wasPressed;
	}
	else if (padCode == PadCode::ButtonDown)
	{
		bool isPressed = GetInstance()->mJoypadObjs[padIndex].padInput.rgdwPOV[0] == 18000;
		bool wasPressed = GetInstance()->mJoypadObjs[padIndex].prevPadInput.rgdwPOV[0] == 18000;
		return !isPressed && wasPressed;
	}

	bool isPressed = (GetInstance()->mJoypadObjs[padIndex].padInput.rgbButtons[(uint32_t)padCode] & 0x80);
	bool wasPressed = (GetInstance()->mJoypadObjs[padIndex].prevPadInput.rgbButtons[(uint32_t)padCode] & 0x80);
	return !isPressed && wasPressed;
}

// 任意のボタンが押されているかを判定
bool JoypadInput::GetAnyButtonDown(const int padIndex)
{
	if (GetisLinkPad() == false) return false;

	for (uint32_t i = 0; i < 16; ++i)
	{
		bool isPressed = (GetInstance()->mJoypadObjs[padIndex].padInput.rgbButtons[i] & 0x80);
		bool wasPressed = (GetInstance()->mJoypadObjs[padIndex].prevPadInput.rgbButtons[i] & 0x80);

		if (isPressed && wasPressed)
		{
			return true;
		}
	}

	for (uint32_t i = 1; i < 5; i++)
	{
		bool isPressed = (GetInstance()->mJoypadObjs[padIndex].padInput.rgdwPOV[0] == i * 9000);
		bool wasPressed = (GetInstance()->mJoypadObjs[padIndex].prevPadInput.rgdwPOV[0] == i * 9000);

		if (isPressed && wasPressed)
		{
			return true;
		}
	}

	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// --- スティック関連 ------------------------------------------------------------------------------ ///
/////////////////////////////////////////////////////////////////////////////////////////////////////////

// スティックの入力状態を取得
Vec2 JoypadInput::GetStick(const PadCode padCode, const float length, const int padIndex)
{
	if (GetisLinkPad() == false) return 0;

	Vec2 stick = 0;
	if (padCode == PadCode::LeftStick)
	{
		stick.x = (float)GetInstance()->mJoypadObjs[padIndex].padInput.lX;
		stick.y = (float)GetInstance()->mJoypadObjs[padIndex].padInput.lY;
	}
	else if (padCode == PadCode::RightStick)
	{
		stick.x = (float)GetInstance()->mJoypadObjs[padIndex].padInput.lRx;
		stick.y = (float)GetInstance()->mJoypadObjs[padIndex].padInput.lRy;
	}

	if (stick.Length() > fabsf(length))
	{
		return stick;
	}

	return 0;
}

// 3Dベクトルとしてスティックの入力状態を取得
Vec3 JoypadInput::GetStickVec3(const PadCode padCode, const float length, const int padIndex)
{
	if (GetisLinkPad() == false) return 0;

	Vec3 stick = 0;
	if (padCode == PadCode::LeftStick)
	{
		stick.x = (float)GetInstance()->mJoypadObjs[padIndex].padInput.lX;
		stick.z = (float)GetInstance()->mJoypadObjs[padIndex].padInput.lY;
	}
	else if (padCode == PadCode::RightStick)
	{
		stick.x = (float)GetInstance()->mJoypadObjs[padIndex].padInput.lRx;
		stick.z = (float)GetInstance()->mJoypadObjs[padIndex].padInput.lRy;
	}

	if (stick.Length() > fabsf(length))
	{
		return stick;
	}

	return 0;
}

// スティックが押された瞬間の状態を取得
Vec2 JoypadInput::GetStickDown(const PadCode padCode, const float length, const int padIndex)
{
	if (GetisLinkPad() == false) return 0;

	Vec2 stick = 0;
	Vec2 prevStick = 0;
	if (padCode == PadCode::LeftStick)
	{
		stick.x = (float)GetInstance()->mJoypadObjs[padIndex].padInput.lX;
		stick.y = (float)GetInstance()->mJoypadObjs[padIndex].padInput.lY;

		prevStick.x = (float)GetInstance()->mJoypadObjs[padIndex].prevPadInput.lX;
		prevStick.y = (float)GetInstance()->mJoypadObjs[padIndex].prevPadInput.lY;
	}
	else if (padCode == PadCode::RightStick)
	{
		stick.x = (float)GetInstance()->mJoypadObjs[padIndex].padInput.lRx;
		stick.y = (float)GetInstance()->mJoypadObjs[padIndex].padInput.lRy;

		prevStick.x = (float)GetInstance()->mJoypadObjs[padIndex].prevPadInput.lRx;
		prevStick.y = (float)GetInstance()->mJoypadObjs[padIndex].prevPadInput.lRy;
	}

	if (!(stick.Length() > fabsf(length)) && (prevStick.Length() > fabsf(length)))
	{
		return prevStick;
	}

	return 0;
}

// スティックが離された瞬間の状態を取得
Vec2 JoypadInput::GetStickUp(const PadCode padCode, const float length, const int padIndex)
{
	if (GetisLinkPad() == false) return 0;

	Vec2 stick = 0;
	Vec2 prevStick = 0;
	if (padCode == PadCode::LeftStick)
	{
		stick.x = (float)GetInstance()->mJoypadObjs[padIndex].padInput.lX;
		stick.y = (float)GetInstance()->mJoypadObjs[padIndex].padInput.lY;

		prevStick.x = (float)GetInstance()->mJoypadObjs[padIndex].prevPadInput.lX;
		prevStick.y = (float)GetInstance()->mJoypadObjs[padIndex].prevPadInput.lY;
	}
	else if (padCode == PadCode::RightStick)
	{
		stick.x = (float)GetInstance()->mJoypadObjs[padIndex].padInput.lRx;
		stick.y = (float)GetInstance()->mJoypadObjs[padIndex].padInput.lRy;

		prevStick.x = (float)GetInstance()->mJoypadObjs[padIndex].prevPadInput.lRx;
		prevStick.y = (float)GetInstance()->mJoypadObjs[padIndex].prevPadInput.lRy;
	}

	if ((stick.Length() > fabsf(length)) && !(prevStick.Length() > fabsf(length)))
	{
		return stick;
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// --- トリガー関連 -------------------------------------------------------------------------------- ///
/////////////////////////////////////////////////////////////////////////////////////////////////////////

// ジョイパッドのトリガー入力を取得
float JoypadInput::GetTrigger(const PadCode padCode, const float length, const int padIndex)
{
	// ジョイパッドが接続されていない場合は0を返す
	if (GetisLinkPad() == false) return 0;

	float trigger = 0;
	if (padCode == PadCode::LeftTrigger)
	{
		trigger = (float)GetInstance()->mJoypadObjs[padIndex].padInput.lZ;
		if (trigger > length)
		{
			return trigger;
		}
	}
	else if (padCode == PadCode::RightTrigger)
	{
		trigger = (float)GetInstance()->mJoypadObjs[padIndex].padInput.lZ;
		if (trigger < -length)
		{
			return trigger;
		}
	}

	return 0;
}

// ジョイパッドのトリガーが押された瞬間の状態を取得
bool JoypadInput::GetTriggerDown(const PadCode padCode, const float length, const int padIndex)
{
	if (padCode == PadCode::LeftTrigger)
	{
		bool isGreater = (float)GetInstance()->mJoypadObjs[padIndex].padInput.lZ > length;
		bool isPrevGreater = (float)GetInstance()->mJoypadObjs[padIndex].prevPadInput.lZ > length;

		if (isGreater && !isPrevGreater)
		{
			return true;
		}
	}
	else if (padCode == PadCode::RightTrigger)
	{
		bool isLess = (float)GetInstance()->mJoypadObjs[padIndex].padInput.lZ < -length;
		bool isPrevLess = (float)GetInstance()->mJoypadObjs[padIndex].prevPadInput.lZ < -length;

		if (isLess && !isPrevLess)
		{
			return true;
		}
	}

	return false;
}

// ジョイパッドのトリガーが離された瞬間の状態を取得
bool JoypadInput::GetTriggerUp(const PadCode padCode, const float length, const int padIndex)
{
	if (padCode == PadCode::LeftTrigger)
	{
		bool isGreater = (float)GetInstance()->mJoypadObjs[padIndex].padInput.lZ > length;
		bool isPrevGreater = (float)GetInstance()->mJoypadObjs[padIndex].prevPadInput.lZ > length;

		if (!isGreater && isPrevGreater)
		{
			return true;
		}
	}
	else if (padCode == PadCode::RightTrigger)
	{
		bool isLess = (float)GetInstance()->mJoypadObjs[padIndex].padInput.lZ < -length;
		bool isPrevLess = (float)GetInstance()->mJoypadObjs[padIndex].padInput.lZ < -length;

		if (!isLess and isPrevLess)
		{
			return true;
		}
	}

	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// --- その他 -------------------------------------------------------------------------------------- ///
/////////////////////////////////////////////////////////////////////////////////////////////////////////

// 挿入フラグ
void JoypadInput::SetisInsertPad(const bool isInsertPad)
{
	JoypadInput::sIsInsertPad = isInsertPad;
}
bool JoypadInput::GetisInsertPad()
{
	return JoypadInput::sIsInsertPad;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// --- プライベートメソッド------------------------------------------------------------------------- ///
/////////////////////////////////////////////////////////////////////////////////////////////////////////

// デバイスを検出し、設定する
void JoypadInput::SetJoyStick()
{
	HRESULT result;
	// ゲームコントローラデバイスを列挙
	result = InputManager::GetInstance()->GetDirectInput()->
		EnumDevices(
			DI8DEVCLASS_GAMECTRL,
			DeviceFindCallBack,
			this,
			DIEDFL_ATTACHEDONLY);
	assert(SUCCEEDED(result));
}

// パッドのインデックスが接続されているか
bool JoypadInput::GetisLinkPad()
{
	return GetInstance()->mJoypadObjs.empty() == false;
}

// デバイスを探すためのコールバック関数
BOOL CALLBACK JoypadInput::DeviceFindCallBack(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext)
{
	HRESULT result;
	JoypadInput* joypadInput = static_cast<JoypadInput*>(pContext);
	Microsoft::WRL::ComPtr<IDirectInputDevice8>  joypad;
	result = InputManager::GetInstance()->GetDirectInput()->
		CreateDevice(pdidInstance->guidInstance, &joypad, nullptr);

	if (FAILED(result)) return DIENUM_CONTINUE;

	DIDEVICEINSTANCE instance;
	joypad->GetDeviceInfo(&instance);
	joypadInput->mJoypadObjs.emplace_back(joypad);

	// データフォーマットを設定
	result = joypad->SetDataFormat(&c_dfDIJoystick2);
	assert(SUCCEEDED(result));

	// デバイスの協調レベルを設定
	result = joypad->SetCooperativeLevel(
		RenderWindow::GetInstance()->GetHwnd(),
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	assert(SUCCEEDED(result));

	// 軸モードを設定
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(DIPROPDWORD);
	diprop.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.diph.dwObj = 0;
	diprop.dwData = DIPROPAXISMODE_ABS;
	joypad->SetProperty(DIPROP_AXISMODE, &diprop.diph);

	// 軸の範囲を設定
	DIPROPRANGE diprg;
	diprg.diph.dwSize = sizeof(DIPROPRANGE);
	diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	diprg.diph.dwHow = DIPH_BYOFFSET;
	diprg.lMin = -1000;
	diprg.lMax = 1000;
	diprg.diph.dwObj = DIJOFS_X;
	joypad->SetProperty(DIPROP_RANGE, &diprg.diph);
	diprg.diph.dwObj = DIJOFS_Y;
	joypad->SetProperty(DIPROP_RANGE, &diprg.diph);
	diprg.diph.dwObj = DIJOFS_Z;
	joypad->SetProperty(DIPROP_RANGE, &diprg.diph);
	diprg.diph.dwObj = DIJOFS_RX;
	joypad->SetProperty(DIPROP_RANGE, &diprg.diph);
	diprg.diph.dwObj = DIJOFS_RY;
	joypad->SetProperty(DIPROP_RANGE, &diprg.diph);
	diprg.diph.dwObj = DIJOFS_RZ;
	joypad->SetProperty(DIPROP_RANGE, &diprg.diph);

	sPadIndex++;

	return DIENUM_CONTINUE;
}