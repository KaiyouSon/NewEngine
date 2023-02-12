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

	StickLeft = 96,
	StickRight = 97,

	TriggerLeft = 98,
	TriggerRight = 99,
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
	static inline bool GetButtonDown(const PadCodo& padCode, const int& padIndex = 0)
	{
		if (padCode == PadCodo::ButtonLeft)
		{
			return (GetInstance()->jyopadObjs[padIndex].padInput.rgdwPOV[0] == 27000) &&
				!(GetInstance()->jyopadObjs[padIndex].prevPadInput.rgdwPOV[0] == 27000);
		}
		else if (padCode == PadCodo::ButtonRight)
		{
			return (GetInstance()->jyopadObjs[padIndex].padInput.rgdwPOV[0] == 9000) &&
				!(GetInstance()->jyopadObjs[padIndex].prevPadInput.rgdwPOV[0] == 9000);
		}
		else if (padCode == PadCodo::ButtonUp)
		{
			return (GetInstance()->jyopadObjs[padIndex].padInput.rgdwPOV[0] == 0) &&
				!(GetInstance()->jyopadObjs[padIndex].prevPadInput.rgdwPOV[0] == 0);
		}
		else if (padCode == PadCodo::ButtonDown)
		{
			return (GetInstance()->jyopadObjs[padIndex].padInput.rgdwPOV[0] == 18000) &&
				!(GetInstance()->jyopadObjs[padIndex].prevPadInput.rgdwPOV[0] == 18000);
		}
		else
		{
			return (GetInstance()->jyopadObjs[padIndex].padInput.rgbButtons[(int)padCode] & 0x80) &&
				!(GetInstance()->jyopadObjs[padIndex].prevPadInput.rgbButtons[(int)padCode] & 0x80);
		}

		return false;
	}
	static inline bool GetButtonUp(const PadCodo& padCode, const int& padIndex = 0)
	{
		if (padCode == PadCodo::ButtonLeft)
		{
			return !(GetInstance()->jyopadObjs[padIndex].padInput.rgdwPOV[0] == 27000) &&
				(GetInstance()->jyopadObjs[padIndex].prevPadInput.rgdwPOV[0] == 27000);
		}
		else if (padCode == PadCodo::ButtonRight)
		{
			return !(GetInstance()->jyopadObjs[padIndex].padInput.rgdwPOV[0] == 9000) &&
				(GetInstance()->jyopadObjs[padIndex].prevPadInput.rgdwPOV[0] == 9000);
		}
		else if (padCode == PadCodo::ButtonUp)
		{
			return !(GetInstance()->jyopadObjs[padIndex].padInput.rgdwPOV[0] == 0) &&
				(GetInstance()->jyopadObjs[padIndex].prevPadInput.rgdwPOV[0] == 0);
		}
		else if (padCode == PadCodo::ButtonDown)
		{
			return !(GetInstance()->jyopadObjs[padIndex].padInput.rgdwPOV[0] == 18000) &&
				(GetInstance()->jyopadObjs[padIndex].prevPadInput.rgdwPOV[0] == 18000);
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
		if (padCode == PadCodo::StickLeft)
		{
			stick.x = (float)GetInstance()->jyopadObjs[padIndex].padInput.lX;
			stick.y = (float)GetInstance()->jyopadObjs[padIndex].padInput.lY;
		}
		else if (padCode == PadCodo::StickLeft)
		{
			stick.x = (float)GetInstance()->jyopadObjs[padIndex].padInput.lRx;
			stick.y = (float)GetInstance()->jyopadObjs[padIndex].padInput.lRy;
		}

		if (stick.Lenght() > lenght)
		{
			return stick.Norm() * lenght;
		}

		return 0;
	}
	static inline Vec2 GetStickDown(const PadCodo& padCode, const float& lenght = 0, const int& padIndex = 0)
	{
		Vec2 stick = 0;
		Vec2 prevStick = 0;
		if (padCode == PadCodo::StickLeft)
		{
			stick.x = (float)GetInstance()->jyopadObjs[padIndex].padInput.lX;
			stick.y = (float)GetInstance()->jyopadObjs[padIndex].padInput.lY;

			prevStick.x = (float)GetInstance()->jyopadObjs[padIndex].prevPadInput.lX;
			prevStick.y = (float)GetInstance()->jyopadObjs[padIndex].prevPadInput.lY;
		}
		else if (padCode == PadCodo::StickRight)
		{
			stick.x = (float)GetInstance()->jyopadObjs[padIndex].padInput.lRx;
			stick.y = (float)GetInstance()->jyopadObjs[padIndex].padInput.lRy;

			prevStick.x = (float)GetInstance()->jyopadObjs[padIndex].prevPadInput.lRx;
			prevStick.y = (float)GetInstance()->jyopadObjs[padIndex].prevPadInput.lRy;
		}

		if ((stick.Lenght() > fabsf(lenght)) && !(prevStick.Lenght() > fabsf(lenght)))
		{
			return stick.Norm() * lenght;
		}

		return 0;
	}
	static inline Vec2 GetStickUp(const PadCodo& padCode, const float& lenght = 0, const int& padIndex = 0)
	{
		Vec2 stick = 0;
		Vec2 prevStick = 0;
		if (padCode == PadCodo::StickLeft)
		{
			stick.x = (float)GetInstance()->jyopadObjs[padIndex].padInput.lX;
			stick.y = (float)GetInstance()->jyopadObjs[padIndex].padInput.lY;

			prevStick.x = (float)GetInstance()->jyopadObjs[padIndex].prevPadInput.lX;
			prevStick.y = (float)GetInstance()->jyopadObjs[padIndex].prevPadInput.lY;
		}
		else if (padCode == PadCodo::StickRight)
		{
			stick.x = (float)GetInstance()->jyopadObjs[padIndex].padInput.lRx;
			stick.y = (float)GetInstance()->jyopadObjs[padIndex].padInput.lRy;

			prevStick.x = (float)GetInstance()->jyopadObjs[padIndex].prevPadInput.lRx;
			prevStick.y = (float)GetInstance()->jyopadObjs[padIndex].prevPadInput.lRy;
		}

		if (!(stick.Lenght() > fabsf(lenght)) && (prevStick.Lenght() > fabsf(lenght)))
		{
			return stick.Norm() * lenght;
		}

		return 0;
	}

	static inline float GetTrigger(const PadCodo& padCode, const float& num = 0, const int& padIndex = 0)
	{
		if (padCode == PadCodo::TriggerLeft)
		{
			if ((float)GetInstance()->jyopadObjs[padIndex].padInput.lZ > num)
			{
				return (float)GetInstance()->jyopadObjs[padIndex].padInput.lZ;
			}
		}
		else if (padCode == PadCodo::TriggerRight)
		{
			if ((float)GetInstance()->jyopadObjs[padIndex].padInput.lZ < -num)
			{
				return (float)GetInstance()->jyopadObjs[padIndex].padInput.lZ * -1;
			}
		}

		return 0;
	}
	static inline bool GetTriggerDown(const PadCodo& padCode, const float& num = 0, const int& padIndex = 0)
	{
		if (padCode == PadCodo::TriggerLeft)
		{
			if (((float)GetInstance()->jyopadObjs[padIndex].padInput.lZ > num) &&
				!((float)GetInstance()->jyopadObjs[padIndex].prevPadInput.lZ > num))
			{
				return (float)GetInstance()->jyopadObjs[padIndex].padInput.lZ;
			}
		}
		else if (padCode == PadCodo::TriggerRight)
		{
			if (((float)GetInstance()->jyopadObjs[padIndex].padInput.lZ < -num) &&
				!((float)GetInstance()->jyopadObjs[padIndex].prevPadInput.lZ < -num))
			{
				return (float)GetInstance()->jyopadObjs[padIndex].padInput.lZ * -1;
			}
		}

		return 0;
	}
	static inline bool GetTriggerUp(const PadCodo& padCode, const float& num = 0, const int& padIndex = 0)
	{
		if (padCode == PadCodo::TriggerLeft)
		{
			if (!((float)GetInstance()->jyopadObjs[padIndex].padInput.lZ > num) &&
				((float)GetInstance()->jyopadObjs[padIndex].prevPadInput.lZ > num))
			{
				return (float)GetInstance()->jyopadObjs[padIndex].padInput.lZ;
			}
		}
		else if (padCode == PadCodo::TriggerRight)
		{
			if (!((float)GetInstance()->jyopadObjs[padIndex].padInput.lZ < -num) &&
				((float)GetInstance()->jyopadObjs[padIndex].prevPadInput.lZ < -num))
			{
				return (float)GetInstance()->jyopadObjs[padIndex].padInput.lZ * -1;
			}
		}

		return 0;
	}

	static inline bool GetisLinkPad(const int& padIndex = 0)
	{
		return GetInstance()->jyopadObjs[padIndex].joypad != nullptr;
	}
};

typedef JoypadInput Pad;