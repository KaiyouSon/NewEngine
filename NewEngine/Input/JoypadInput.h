#pragma once
#include "MathUtil.h"
#include "Singleton.h"
#include <array>
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

	struct JoypadObj
	{
		Microsoft::WRL::ComPtr<IDirectInputDevice8> joypad;
		DIJOYSTATE2 padInput;
		DIJOYSTATE2 prevPadInput;
	};

	std::array<JoypadObj, 2> jyopadObjs;
	static int padIndex;

	static BOOL CALLBACK DeviceFindCallBack(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext);
private:
	JoypadInput();

public:
	void Init();
	void Update();

	static inline bool GetButton(const PadCodo& padCode, const int& padIndex = 0)
	{
		if (padCode == PadCodo::ButtonLeft)
		{
			return GetInstance()->jyopadObjs[padIndex].padInput.rgdwPOV[0] == 27000;
		}
		else if (padCode == PadCodo::ButtonRight)
		{
			return GetInstance()->jyopadObjs[padIndex].padInput.rgdwPOV[0] == 9000;
		}
		else if (padCode == PadCodo::ButtonUp)
		{
			return GetInstance()->jyopadObjs[padIndex].padInput.rgdwPOV[0] == 0;
		}
		else if (padCode == PadCodo::ButtonDown)
		{
			return GetInstance()->jyopadObjs[padIndex].padInput.rgdwPOV[0] == 18000;
		}
		else
		{
			return GetInstance()->jyopadObjs[padIndex].padInput.rgbButtons[(int)padCode] & 0x80;
		}

		return false;
	}
	static inline bool GetButtonTrigger(const PadCodo& padCode, const int& padIndex = 0)
	{
		if (padCode == PadCodo::ButtonLeft)
		{
			return (GetInstance()->jyopadObjs[padIndex].padInput.rgdwPOV[0] == 27000) &&
				!(GetInstance()->jyopadObjs[padIndex].padInput.rgdwPOV[0] == 27000);
		}
		else if (padCode == PadCodo::ButtonRight)
		{
			return (GetInstance()->jyopadObjs[padIndex].padInput.rgdwPOV[0] == 9000) &&
				!(GetInstance()->jyopadObjs[padIndex].padInput.rgdwPOV[0] == 9000);
		}
		else if (padCode == PadCodo::ButtonUp)
		{
			return (GetInstance()->jyopadObjs[padIndex].padInput.rgdwPOV[0] == 0) &&
				!(GetInstance()->jyopadObjs[padIndex].padInput.rgdwPOV[0] == 0);
		}
		else if (padCode == PadCodo::ButtonDown)
		{
			return (GetInstance()->jyopadObjs[padIndex].padInput.rgdwPOV[0] == 18000) &&
				!(GetInstance()->jyopadObjs[padIndex].padInput.rgdwPOV[0] == 18000);
		}
		else
		{
			return (GetInstance()->jyopadObjs[padIndex].padInput.rgbButtons[(int)padCode] & 0x80) &&
				!(GetInstance()->jyopadObjs[padIndex].prevPadInput.rgbButtons[(int)padCode] & 0x80);
		}

		return false;
	}
	static inline bool GetButtonReleased(const PadCodo& padCode, const int& padIndex = 0)
	{
		if (padCode == PadCodo::ButtonLeft)
		{
			return !(GetInstance()->jyopadObjs[padIndex].padInput.rgdwPOV[0] == 27000) &&
				(GetInstance()->jyopadObjs[padIndex].padInput.rgdwPOV[0] == 27000);
		}
		else if (padCode == PadCodo::ButtonRight)
		{
			return !(GetInstance()->jyopadObjs[padIndex].padInput.rgdwPOV[0] == 9000) &&
				(GetInstance()->jyopadObjs[padIndex].padInput.rgdwPOV[0] == 9000);
		}
		else if (padCode == PadCodo::ButtonUp)
		{
			return !(GetInstance()->jyopadObjs[padIndex].padInput.rgdwPOV[0] == 0) &&
				(GetInstance()->jyopadObjs[padIndex].padInput.rgdwPOV[0] == 0);
		}
		else if (padCode == PadCodo::ButtonDown)
		{
			return !(GetInstance()->jyopadObjs[padIndex].padInput.rgdwPOV[0] == 18000) &&
				(GetInstance()->jyopadObjs[padIndex].padInput.rgdwPOV[0] == 18000);
		}
		else
		{
			return !(GetInstance()->jyopadObjs[padIndex].padInput.rgbButtons[(int)padCode] & 0x80) &&
				(GetInstance()->jyopadObjs[padIndex].prevPadInput.rgbButtons[(int)padCode] & 0x80);
		}

		return false;
	}

	static inline Vec2 GetStick(const PadCodo& padCode, const float& lenght = 0, const int& padIndex = 0)
	{
		Vec2 stick = 0;
		if (padCode == PadCodo::LeftStick)
		{
			stick.x = (float)GetInstance()->jyopadObjs[padIndex].padInput.lX;
			stick.y = (float)GetInstance()->jyopadObjs[padIndex].padInput.lY;

			if (stick.Lenght() > lenght)
			{
				return stick.Norm() * lenght;
			}
		}
		else if (padCode == PadCodo::LeftStick)
		{
			stick.x = (float)GetInstance()->jyopadObjs[padIndex].padInput.lRx;
			stick.y = (float)GetInstance()->jyopadObjs[padIndex].padInput.lRy;

			if (stick.Lenght() > lenght)
			{
				return stick.Norm() * lenght;
			}
		}

		return 0;
	}
	static inline bool GetStickTrigger(const int& padCode, const Vec2& num = 0, const int& padIndex = 0)
	{
		if (padCode == 96)
		{
			return
				(((float)GetInstance()->jyopadObjs[padIndex].padInput.lX > +num.x) && !((float)GetInstance()->jyopadObjs[padIndex].prevPadInput.lX > +num.x)) ||
				(((float)GetInstance()->jyopadObjs[padIndex].padInput.lX < -num.x) && !((float)GetInstance()->jyopadObjs[padIndex].prevPadInput.lX < -num.x)) ||
				(((float)GetInstance()->jyopadObjs[padIndex].padInput.lY > +num.y) && !((float)GetInstance()->jyopadObjs[padIndex].prevPadInput.lY > +num.y)) ||
				(((float)GetInstance()->jyopadObjs[padIndex].padInput.lY < -num.y) && !((float)GetInstance()->jyopadObjs[padIndex].prevPadInput.lY < -num.y));
		}
		else if (padCode == 97)
		{
			return
				(((float)GetInstance()->jyopadObjs[padIndex].padInput.lRx > +num.x) && !((float)GetInstance()->jyopadObjs[padIndex].prevPadInput.lRx > +num.x)) ||
				(((float)GetInstance()->jyopadObjs[padIndex].padInput.lRx < -num.x) && !((float)GetInstance()->jyopadObjs[padIndex].prevPadInput.lRx < -num.x)) ||
				(((float)GetInstance()->jyopadObjs[padIndex].padInput.lRy > +num.y) && !((float)GetInstance()->jyopadObjs[padIndex].prevPadInput.lRy > +num.y)) ||
				(((float)GetInstance()->jyopadObjs[padIndex].padInput.lRy < -num.y) && !((float)GetInstance()->jyopadObjs[padIndex].prevPadInput.lRy < -num.y));
		}

		return false;
	}
	static inline bool GetStickReleased(const int& padCode, const Vec2& num = 0, const int& padIndex = 0)
	{
		if (padCode == 96)
		{
			return
				!(((float)GetInstance()->jyopadObjs[padIndex].padInput.lX > +num.x) && ((float)GetInstance()->jyopadObjs[padIndex].prevPadInput.lX > +num.x)) ||
				!(((float)GetInstance()->jyopadObjs[padIndex].padInput.lX < -num.x) && ((float)GetInstance()->jyopadObjs[padIndex].prevPadInput.lX < -num.x)) ||
				!(((float)GetInstance()->jyopadObjs[padIndex].padInput.lY > +num.y) && ((float)GetInstance()->jyopadObjs[padIndex].prevPadInput.lY > +num.y)) ||
				!(((float)GetInstance()->jyopadObjs[padIndex].padInput.lY < -num.y) && ((float)GetInstance()->jyopadObjs[padIndex].prevPadInput.lY < -num.y));
		}
		else if (padCode == 97)
		{
			return
				!(((float)GetInstance()->jyopadObjs[padIndex].padInput.lRx > +num.x) && ((float)GetInstance()->jyopadObjs[padIndex].prevPadInput.lRx > +num.x)) ||
				!(((float)GetInstance()->jyopadObjs[padIndex].padInput.lRx < -num.x) && ((float)GetInstance()->jyopadObjs[padIndex].prevPadInput.lRx < -num.x)) ||
				!(((float)GetInstance()->jyopadObjs[padIndex].padInput.lRy > +num.y) && ((float)GetInstance()->jyopadObjs[padIndex].prevPadInput.lRy > +num.y)) ||
				!(((float)GetInstance()->jyopadObjs[padIndex].padInput.lRy < -num.y) && ((float)GetInstance()->jyopadObjs[padIndex].prevPadInput.lRy < -num.y));
		}

		return false;
	}

	static inline bool GetTrigger(const int& padCode, const float& num = 0, const int& padIndex = 0)
	{
		if (padCode == 98)
		{
			return ((float)GetInstance()->jyopadObjs[padIndex].padInput.lZ > num) ? (float)GetInstance()->jyopadObjs[padIndex].padInput.lZ : 0;
		}
		else if (padCode == 99)
		{
			return ((float)GetInstance()->jyopadObjs[padIndex].padInput.lZ < num) ? (float)GetInstance()->jyopadObjs[padIndex].padInput.lZ : 0;
		}

		return false;
	}
	static inline bool GetTriggerDown(const int& padCode, const float& num = 0, const int& padIndex = 0)
	{
		if (padCode == 98)
		{
			return ((float)GetInstance()->jyopadObjs[padIndex].padInput.lZ > num) &&
				!((float)GetInstance()->jyopadObjs[padIndex].prevPadInput.lZ > num);
		}
		else if (padCode == 99)
		{
			return ((float)GetInstance()->jyopadObjs[padIndex].padInput.lZ < num) &&
				!((float)GetInstance()->jyopadObjs[padIndex].prevPadInput.lZ < num);
		}
	}
	static inline bool GetTriggerUp(const int& padCode, const float& num = 0, const int& padIndex = 0)
	{
		if (padCode == 98)
		{
			return !((float)GetInstance()->jyopadObjs[padIndex].padInput.lZ > num) &&
				((float)GetInstance()->jyopadObjs[padIndex].prevPadInput.lZ > num);
		}
		else if (padCode == 99)
		{
			return !((float)GetInstance()->jyopadObjs[padIndex].padInput.lZ < num) &&
				((float)GetInstance()->jyopadObjs[padIndex].prevPadInput.lZ < num);
		}
	}

	static inline bool GetisLinkPad(const int& padIndex = 0) { return GetInstance()->jyopadObjs[padIndex].joypad != nullptr; }
};

typedef JoypadInput Pad;