#pragma once
#include "MathUtil.h"
#include "Singleton.h"
#include <dinput.h>
#include <wrl.h>

template<typename T> class Singleton;

class JoypadInput : public Singleton<JoypadInput>
{
private:
	friend Singleton<JoypadInput>;
	Microsoft::WRL::ComPtr<IDirectInputDevice8> joypad;
	DIJOYSTATE2 padInput;
	DIJOYSTATE2 prevPadInput;

	static BOOL CALLBACK DeviceFindCallBack(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext);
private:
	JoypadInput();

public:
	void Init();
	void Update();

	static inline bool GetButton(const int& buttonNumber)
	{
		return GetInstance()->padInput.rgbButtons[buttonNumber] & 0x80;
	}
	static inline bool GetButtonTrigger(const int& buttonNumber)
	{
		return (GetInstance()->padInput.rgbButtons[buttonNumber] & 0x80) &&
			!(GetInstance()->prevPadInput.rgbButtons[buttonNumber] & 0x80);
	}
	static inline bool GetButtonReleased(const int& buttonNumber)
	{
		return !(GetInstance()->padInput.rgbButtons[buttonNumber] & 0x80) &&
			(GetInstance()->prevPadInput.rgbButtons[buttonNumber] & 0x80);
	}
	static inline Vec2 GetLeftStick()
	{
		return Vec2((float)GetInstance()->padInput.lX, (float)GetInstance()->padInput.lY);
	}
	static inline Vec2 GetRightStick()
	{
		return Vec2((float)GetInstance()->padInput.lRx, (float)GetInstance()->padInput.lRy);
	}
};

typedef JoypadInput Pad;