#include "InputManager.h"
#include "JoypadInput.h"
#include "RenderWindow.h"
#include <cassert>
#include <dbt.h>

bool JoypadInput::sIsInsertPad = false;
uint32_t JoypadInput::sPadIndex = 0;

// 繧ｦ繧｣繝ｳ繝峨え繝励Ο繧ｷ繝ｼ繧ｸ繝｣
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

	// 謚懊″蟾ｮ縺玲､懃衍
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

		// 繧ｸ繝ｧ繧､繝代ャ繝域ュ蝣ｱ縺ｮ蜿門ｾ鈴幕蟋・
		mJoypadObjs[i].joypad->Acquire();

		// 繝・ヰ繧､繧ｹ縺ｮ繝昴・繝ｪ繝ｳ繧ｰ繧定｡後≧
		mJoypadObjs[i].joypad->Poll();

		// 譛譁ｰ縺ｮ繝代ャ繝域ュ蝣ｱ縺縺｣縺溘ｂ縺ｮ縺ｯ1繝輔Ξ繝ｼ繝蜑阪・繧ｭ繝ｼ繝懊・繝画ュ蝣ｱ縺ｨ縺励※菫晏ｭ・
		mJoypadObjs[i].prevPadInput = mJoypadObjs[i].padInput;

		// 譛譁ｰ縺ｮ繧ｸ繝ｧ繧､繝代ャ繝域ュ蝣ｱ繧貞叙蠕励☆繧・
		mJoypadObjs[i].joypad->GetDeviceState(sizeof(mJoypadObjs[i].padInput), &mJoypadObjs[i].padInput);
	}
}

// 繧ｳ繝ｳ繝医Ο繝ｼ繝ｩ繝ｼ謗･邯壹＠縺滓凾縺ｮ繧ｳ繝ｼ繝ｫ繝舌ャ繧ｯ
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

	// 蜈･蜉帙ョ繝ｼ繧ｿ蠖｢蠑上・繧ｻ繝・ヨ
	result = joypad->SetDataFormat(&c_dfDIJoystick2);
	assert(SUCCEEDED(result));

	// 謗剃ｻ門宛蠕｡繝ｬ繝吶Ν縺ｮ繧ｻ繝・ヨ
	result = joypad->SetCooperativeLevel(
		RenderWindow::GetInstance()->GetHwnd(),
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	assert(SUCCEEDED(result));

	// 霆ｸ縺ｮ繝｢繝ｼ繝峨ｒ邨ｶ蟇ｾ霆ｸ縺ｫ險ｭ螳・
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(DIPROPDWORD);
	diprop.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.diph.dwObj = 0;
	diprop.dwData = DIPROPAXISMODE_ABS;
	joypad->SetProperty(DIPROP_AXISMODE, &diprop.diph);

	//霆ｸ縺ｮ蠖薙◆繧翫・遽・峇繧定ｨｭ螳・
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

// 繧ｳ繝ｳ繝医Ο繝ｼ繝ｩ繝ｼ繧呈磁邯壹☆繧句・逅・
void JoypadInput::SetJoyStick()
{
	HRESULT result;
	// 繧ｸ繝ｧ繧､繝代ャ繝医ョ繝舌う繧ｹ縺ｮ蛻玲嫌
	result = InputManager::GetInstance()->GetDirectInput()->
		EnumDevices(
			DI8DEVCLASS_GAMECTRL,
			DeviceFindCallBack,
			this,
			DIEDFL_ATTACHEDONLY);
	assert(SUCCEEDED(result));
}

// 繝懊ち繝ｳ繧呈款縺励※繧矩俣
bool JoypadInput::GetButton(const PadCode padCode, const int padIndex)
{
	// 謗･邯壹＠縺ｦ縺・ｋ縺・
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

// 繝懊ち繝ｳ繧呈款縺励◆迸ｬ髢・
bool JoypadInput::GetButtonDown(const PadCode padCode, const int padIndex)
{
	// 謗･邯壹＠縺ｦ縺・ｋ縺・
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

// 繝懊ち繝ｳ繧帝屬縺励◆迸ｬ髢・
bool JoypadInput::GetButtonUp(const PadCode padCode, const int padIndex)
{
	// 謗･邯壹＠縺ｦ縺・ｋ縺・
	if (GetisLinkPad() == false) return false;

	return !(GetInstance()->mJoypadObjs[padIndex].padInput.rgbButtons[(uint32_t)padCode] & 0x80) &&
		(GetInstance()->mJoypadObjs[padIndex].prevPadInput.rgbButtons[(uint32_t)padCode] & 0x80);
}

// 菴輔°縺ｮ繝懊ち繝ｳ繧呈款縺励◆迸ｬ髢・
bool JoypadInput::GetAnyButtonDown(const int padIndex)
{
	// 謗･邯壹＠縺ｦ縺・ｋ縺・
	if (GetisLinkPad() == false) return false;

	// 繝懊ち繝ｳ縺ｮ迥ｶ諷九ｒ譖ｴ譁ｰ
	for (uint32_t i = 0; i < 16; ++i)
	{
		bool down = (GetInstance()->mJoypadObjs[padIndex].padInput.rgbButtons[i] & 0x80);
		bool up = !(GetInstance()->mJoypadObjs[padIndex].prevPadInput.rgbButtons[i] & 0x80);

		if (down == true && up == true)
		{
			return true;
		}
	}

	for (uint32_t i = 1; i < 5; i++)
	{
		bool down = (GetInstance()->mJoypadObjs[padIndex].padInput.rgdwPOV[0] == i * 9000);
		bool up = !(GetInstance()->mJoypadObjs[padIndex].prevPadInput.rgdwPOV[0] == i * 9000);

		if (down == true && up == true)
		{
			return true;
		}
	}

	return false;
}

// 繧ｹ繝・ぅ繝・け繧貞偵＠縺ｦ縺・ｋ髢・
Vec2 JoypadInput::GetStick(const PadCode padCode, const float length, const int padIndex)
{
	// 謗･邯壹＠縺ｦ縺・ｋ縺・
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

	// 謖・ｮ壹＠縺滄聞縺輔ｒ雜・∴縺溷ｴ蜷医↓縺励°霑斐＆縺ｪ縺・
	if (stick.Length() > fabsf(length))
	{
		return stick;
	}

	return 0;
}
Vec3 JoypadInput::GetStickVec3(const PadCode padCode, const float length, const int padIndex)
{
	// 謗･邯壹＠縺ｦ縺・ｋ縺・
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

	// 謖・ｮ壹＠縺滄聞縺輔ｒ雜・∴縺溷ｴ蜷医↓縺励°霑斐＆縺ｪ縺・
	if (stick.Length() > fabsf(length))
	{
		return stick;
	}

	return 0;
}

// 繧ｹ繝・ぅ繝・け繧貞偵＠縺溽椪髢・
Vec2 JoypadInput::GetStickDown(const PadCode padCode, const float length, const int padIndex)
{
	// 謗･邯壹＠縺ｦ縺・ｋ縺・
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

	// 謖・ｮ壹＠縺滄聞縺輔ｒ雜・∴縺溷ｴ蜷医↓縺励°霑斐＆縺ｪ縺・
	if (!(stick.Length() > fabsf(length)) && (prevStick.Length() > fabsf(length)))
	{
		return prevStick;
	}

	return 0;
}

// 繧ｹ繝・ぅ繝・け繧帝屬縺励◆迸ｬ髢・
Vec2 JoypadInput::GetStickUp(const PadCode padCode, const float length, const int padIndex)
{
	// 謗･邯壹＠縺ｦ縺・ｋ縺・
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

	// 謖・ｮ壹＠縺滄聞縺輔ｒ雜・∴縺溷ｴ蜷医↓縺励°霑斐＆縺ｪ縺・
	if (!(stick.Length() > fabsf(length)) && (prevStick.Length() > fabsf(length)))
	{
		return stick;
	}

	return 0;
}

// 繝医Μ繧ｬ繝ｼ繧呈款縺励◆繧矩俣
float JoypadInput::GetTrigger(const PadCode padCode, const float length, const int padIndex)
{
	// 謗･邯壹＠縺ｦ縺・ｋ縺・
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

// 繝医Μ繧ｬ繝ｼ繧呈款縺励◆迸ｬ髢・
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
		bool isPrevLess = (float)GetInstance()->mJoypadObjs[padIndex].padInput.lZ < -length;

		if (isLess && !isPrevLess)
		{
			return true;
		}
	}

	return false;
}

// 繝医Μ繧ｬ繝ｼ繧帝屬縺励◆迸ｬ髢・
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

		if (!isLess && isPrevLess)
		{
			return true;
		}
	}

	return false;
}

// 繧ｳ繝ｳ繝医Ο繝ｼ繝ｩ繝ｼ謗･邯壹＠縺ｦ縺・ｋ縺・
bool JoypadInput::GetisLinkPad()
{
	// 莉ｮ
	return GetInstance()->mJoypadObjs.size() > 0;
}
