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

	static inline Vec2 GetLeftStick(const Vec2& num = 0)
	{
		Vec2 stick = 0;
		if ((float)GetInstance()->padInput.lX > num.x || (float)GetInstance()->padInput.lX < -num.x)
		{
			stick.x = (float)GetInstance()->padInput.lX;
		}
		if ((float)GetInstance()->padInput.lY > num.y || (float)GetInstance()->padInput.lY < -num.y)
		{
			stick.y = (float)GetInstance()->padInput.lY;
		}
		return stick;
	}
	static inline Vec2 GetRightStick(const Vec2& num = 0)
	{
		Vec2 stick = 0;
		if ((float)GetInstance()->padInput.lRx > num.x || (float)GetInstance()->padInput.lRx < -num.x)
		{
			stick.x = (float)GetInstance()->padInput.lRx;
		}
		if ((float)GetInstance()->padInput.lRy > num.y || (float)GetInstance()->padInput.lRy < -num.y)
		{
			stick.y = (float)GetInstance()->padInput.lRy;
		}
		return stick;
	}

	static inline bool GetZL(const float& num = 0)
	{
		return ((float)GetInstance()->padInput.lZ > num) ? (float)GetInstance()->padInput.lZ : 0;
	}
	static inline bool GetZR(const float& num = 0)
	{
		return ((float)GetInstance()->padInput.lZ < num) ? (float)GetInstance()->padInput.lZ : 0;
	}
	static inline bool GetZLTrigger(const float& num = 0)
	{
		return ((float)GetInstance()->padInput.lZ > num) &&
			!((float)GetInstance()->prevPadInput.lZ > num);
	}
	static inline bool GetZRTrigger(const float& num = 0)
	{
		return ((float)GetInstance()->padInput.lZ < num) &&
			!((float)GetInstance()->prevPadInput.lZ < num);
	}

	static inline bool GetisLinkPad() { return GetInstance()->joypad != nullptr; }
};

typedef JoypadInput Pad;