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

	static inline bool GetButton(const int& padCode)
	{
		return GetInstance()->padInput.rgbButtons[padCode] & 0x80;
	}
	static inline bool GetButtonTrigger(const int& padCode)
	{
		return (GetInstance()->padInput.rgbButtons[padCode] & 0x80) &&
			!(GetInstance()->prevPadInput.rgbButtons[padCode] & 0x80);
	}
	static inline bool GetButtonReleased(const int& padCode)
	{
		return !(GetInstance()->padInput.rgbButtons[padCode] & 0x80) &&
			(GetInstance()->prevPadInput.rgbButtons[padCode] & 0x80);
	}

	static inline Vec2 GetStick(const int& padCode, const Vec2& num = 0)
	{
		Vec2 stick = 0;
		if (padCode == 96)
		{
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
		else if (padCode == 97)
		{
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

		return stick;
	}
	static inline bool GetStickTrigger(const int& padCode, const Vec2& num = 0)
	{
		if (padCode == 96)
		{
			return
				(((float)GetInstance()->padInput.lX > +num.x) && !((float)GetInstance()->prevPadInput.lX > +num.x)) ||
				(((float)GetInstance()->padInput.lX < -num.x) && !((float)GetInstance()->prevPadInput.lX < -num.x)) ||
				(((float)GetInstance()->padInput.lY > +num.y) && !((float)GetInstance()->prevPadInput.lY > +num.y)) ||
				(((float)GetInstance()->padInput.lY < -num.y) && !((float)GetInstance()->prevPadInput.lY < -num.y));
		}
		else if (padCode == 97)
		{
			return
				(((float)GetInstance()->padInput.lRx > +num.x) && !((float)GetInstance()->prevPadInput.lRx > +num.x)) ||
				(((float)GetInstance()->padInput.lRx < -num.x) && !((float)GetInstance()->prevPadInput.lRx < -num.x)) ||
				(((float)GetInstance()->padInput.lRy > +num.y) && !((float)GetInstance()->prevPadInput.lRy > +num.y)) ||
				(((float)GetInstance()->padInput.lRy < -num.y) && !((float)GetInstance()->prevPadInput.lRy < -num.y));
		}

		return false;
	}
	static inline bool GetStickReleased(const int& padCode, const Vec2& num = 0)
	{
		if (padCode == 96)
		{
			return
				!(((float)GetInstance()->padInput.lX > +num.x) && ((float)GetInstance()->prevPadInput.lX > +num.x)) ||
				!(((float)GetInstance()->padInput.lX < -num.x) && ((float)GetInstance()->prevPadInput.lX < -num.x)) ||
				!(((float)GetInstance()->padInput.lY > +num.y) && ((float)GetInstance()->prevPadInput.lY > +num.y)) ||
				!(((float)GetInstance()->padInput.lY < -num.y) && ((float)GetInstance()->prevPadInput.lY < -num.y));
		}
		else if (padCode == 97)
		{
			return
				!(((float)GetInstance()->padInput.lRx > +num.x) && ((float)GetInstance()->prevPadInput.lRx > +num.x)) ||
				!(((float)GetInstance()->padInput.lRx < -num.x) && ((float)GetInstance()->prevPadInput.lRx < -num.x)) ||
				!(((float)GetInstance()->padInput.lRy > +num.y) && ((float)GetInstance()->prevPadInput.lRy > +num.y)) ||
				!(((float)GetInstance()->padInput.lRy < -num.y) && ((float)GetInstance()->prevPadInput.lRy < -num.y));
		}

		return false;
	}

	static inline bool GetTrigger(const int& padCode, const float& num = 0)
	{
		if (padCode == 98)
		{
			return ((float)GetInstance()->padInput.lZ > num) ? (float)GetInstance()->padInput.lZ : 0;
		}
		else if (padCode == 99)
		{
			return ((float)GetInstance()->padInput.lZ < num) ? (float)GetInstance()->padInput.lZ : 0;
		}

		return false;
	}
	static inline bool GetTriggerDown(const int& padCode, const float& num = 0)
	{
		if (padCode == 98)
		{
			return ((float)GetInstance()->padInput.lZ > num) &&
				!((float)GetInstance()->prevPadInput.lZ > num);
		}
		else if (padCode == 99)
		{
			return ((float)GetInstance()->padInput.lZ < num) &&
				!((float)GetInstance()->prevPadInput.lZ < num);
		}
	}
	static inline bool GetTriggerUp(const int& padCode, const float& num = 0)
	{
		if (padCode == 98)
		{
			return !((float)GetInstance()->padInput.lZ > num) &&
				((float)GetInstance()->prevPadInput.lZ > num);
		}
		else if (padCode == 99)
		{
			return !((float)GetInstance()->padInput.lZ < num) &&
				((float)GetInstance()->prevPadInput.lZ < num);
		}
	}

	static inline bool GetisLinkPad() { return GetInstance()->joypad != nullptr; }
};

typedef JoypadInput Pad;