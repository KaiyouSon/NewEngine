#pragma once
#include "MathUtil.h"
#include "Singleton.h"
#include <dinput.h>
#include <wrl.h>

enum class PadCodo
{
	ButtonA = 0,
	ButtonB = 1,
	ButtonX = 2,
	ButtonY = 3,
	ButtonL1 = 4,
	ButtonR1 = 5,

	ButtonLeft = 20,
	ButtonRight = 21,
	ButtonUp = 22,
	ButtonDown = 23,

	LeftStick = 96,
	RightStick = 97,
	LeftTrigger = 98,
	RightTrigger = 99,
};

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

	static inline bool GetButton(const PadCodo& padCode)
	{
		if (padCode == PadCodo::ButtonLeft)
		{
			return GetInstance()->padInput.rgdwPOV[0] == 27000;
		}
		else if (padCode == PadCodo::ButtonRight)
		{
			return GetInstance()->padInput.rgdwPOV[0] == 9000;
		}
		else if (padCode == PadCodo::ButtonUp)
		{
			return GetInstance()->padInput.rgdwPOV[0] == 0;
		}
		else if (padCode == PadCodo::ButtonDown)
		{
			return GetInstance()->padInput.rgdwPOV[0] == 18000;
		}
		else
		{
			return GetInstance()->padInput.rgbButtons[(int)padCode] & 0x80;
		}

		return false;
	}
	static inline bool GetButtonTrigger(const PadCodo& padCode)
	{
		if (padCode == PadCodo::ButtonLeft)
		{
			return (GetInstance()->padInput.rgdwPOV[0] == 27000) &&
				!(GetInstance()->padInput.rgdwPOV[0] == 27000);
		}
		else if (padCode == PadCodo::ButtonRight)
		{
			return (GetInstance()->padInput.rgdwPOV[0] == 9000) &&
				!(GetInstance()->padInput.rgdwPOV[0] == 9000);
		}
		else if (padCode == PadCodo::ButtonUp)
		{
			return (GetInstance()->padInput.rgdwPOV[0] == 0) &&
				!(GetInstance()->padInput.rgdwPOV[0] == 0);
		}
		else if (padCode == PadCodo::ButtonDown)
		{
			return (GetInstance()->padInput.rgdwPOV[0] == 18000) &&
				!(GetInstance()->padInput.rgdwPOV[0] == 18000);
		}
		else
		{
			return (GetInstance()->padInput.rgbButtons[(int)padCode] & 0x80) &&
				!(GetInstance()->prevPadInput.rgbButtons[(int)padCode] & 0x80);
		}

		return false;
	}
	static inline bool GetButtonReleased(const PadCodo& padCode)
	{
		if (padCode == PadCodo::ButtonLeft)
		{
			return !(GetInstance()->padInput.rgdwPOV[0] == 27000) &&
				(GetInstance()->padInput.rgdwPOV[0] == 27000);
		}
		else if (padCode == PadCodo::ButtonRight)
		{
			return !(GetInstance()->padInput.rgdwPOV[0] == 9000) &&
				(GetInstance()->padInput.rgdwPOV[0] == 9000);
		}
		else if (padCode == PadCodo::ButtonUp)
		{
			return !(GetInstance()->padInput.rgdwPOV[0] == 0) &&
				(GetInstance()->padInput.rgdwPOV[0] == 0);
		}
		else if (padCode == PadCodo::ButtonDown)
		{
			return !(GetInstance()->padInput.rgdwPOV[0] == 18000) &&
				(GetInstance()->padInput.rgdwPOV[0] == 18000);
		}
		else
		{
			return !(GetInstance()->padInput.rgbButtons[(int)padCode] & 0x80) &&
				(GetInstance()->prevPadInput.rgbButtons[(int)padCode] & 0x80);
		}

		return false;
	}

	static inline Vec2 GetStick(const PadCodo& padCode, const float& lenght = 0)
	{
		Vec2 stick = 0;
		if (padCode == PadCodo::LeftStick)
		{
			stick.x = (float)GetInstance()->padInput.lX;
			stick.y = (float)GetInstance()->padInput.lY;

			if (stick.Lenght() > lenght)
			{
				return stick.Norm() * lenght;
			}
		}
		else if (padCode == PadCodo::LeftStick)
		{
			stick.x = (float)GetInstance()->padInput.lRx;
			stick.y = (float)GetInstance()->padInput.lRy;

			if (stick.Lenght() > lenght)
			{
				return stick.Norm() * lenght;
			}
		}

		return 0;
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