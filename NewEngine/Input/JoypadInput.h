#pragma once
#include "MathUtil.h"
#include "Singleton.h"
#include <vector>
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

	std::vector<JoypadObj> joypadObjs;
	static bool isInsertPad;
	static int padIndex;

	static BOOL CALLBACK DeviceFindCallBack(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext);
private:
	JoypadInput();
	void SetJoyStick();

public:
	void Init();
	void Update();

	static void SetisInsertPad(const bool isInsertPad) { JoypadInput::isInsertPad = isInsertPad; }
	static inline bool GetisInsertPad() { return JoypadInput::isInsertPad; }

	static inline bool GetButton(const PadCodo& padCode, const int& padIndex = 0)
	{
		if (padCode == PadCodo::ButtonLeft)
		{
			return GetInstance()->joypadObjs[padIndex].padInput.rgdwPOV[0] == 27000;
		}
		else if (padCode == PadCodo::ButtonRight)
		{
			return GetInstance()->joypadObjs[padIndex].padInput.rgdwPOV[0] == 9000;
		}
		else if (padCode == PadCodo::ButtonUp)
		{
			return GetInstance()->joypadObjs[padIndex].padInput.rgdwPOV[0] == 0;
		}
		else if (padCode == PadCodo::ButtonDown)
		{
			return GetInstance()->joypadObjs[padIndex].padInput.rgdwPOV[0] == 18000;
		}
		else
		{
			return GetInstance()->joypadObjs[padIndex].padInput.rgbButtons[(int)padCode] & 0x80;
		}

		return false;
	}
	static inline bool GetButtonDown(const PadCodo& padCode, const int& padIndex = 0)
	{
		if (padCode == PadCodo::ButtonLeft)
		{
			return (GetInstance()->joypadObjs[padIndex].padInput.rgdwPOV[0] == 27000) &&
				!(GetInstance()->joypadObjs[padIndex].prevPadInput.rgdwPOV[0] == 27000);
		}
		else if (padCode == PadCodo::ButtonRight)
		{
			return (GetInstance()->joypadObjs[padIndex].padInput.rgdwPOV[0] == 9000) &&
				!(GetInstance()->joypadObjs[padIndex].prevPadInput.rgdwPOV[0] == 9000);
		}
		else if (padCode == PadCodo::ButtonUp)
		{
			return (GetInstance()->joypadObjs[padIndex].padInput.rgdwPOV[0] == 0) &&
				!(GetInstance()->joypadObjs[padIndex].prevPadInput.rgdwPOV[0] == 0);
		}
		else if (padCode == PadCodo::ButtonDown)
		{
			return (GetInstance()->joypadObjs[padIndex].padInput.rgdwPOV[0] == 18000) &&
				!(GetInstance()->joypadObjs[padIndex].prevPadInput.rgdwPOV[0] == 18000);
		}
		else
		{
			return (GetInstance()->joypadObjs[padIndex].padInput.rgbButtons[(int)padCode] & 0x80) &&
				!(GetInstance()->joypadObjs[padIndex].prevPadInput.rgbButtons[(int)padCode] & 0x80);
		}

		return false;
	}
	static inline bool GetButtonUp(const PadCodo& padCode, const int& padIndex = 0)
	{
		if (padCode == PadCodo::ButtonLeft)
		{
			return !(GetInstance()->joypadObjs[padIndex].padInput.rgdwPOV[0] == 27000) &&
				(GetInstance()->joypadObjs[padIndex].prevPadInput.rgdwPOV[0] == 27000);
		}
		else if (padCode == PadCodo::ButtonRight)
		{
			return !(GetInstance()->joypadObjs[padIndex].padInput.rgdwPOV[0] == 9000) &&
				(GetInstance()->joypadObjs[padIndex].prevPadInput.rgdwPOV[0] == 9000);
		}
		else if (padCode == PadCodo::ButtonUp)
		{
			return !(GetInstance()->joypadObjs[padIndex].padInput.rgdwPOV[0] == 0) &&
				(GetInstance()->joypadObjs[padIndex].prevPadInput.rgdwPOV[0] == 0);
		}
		else if (padCode == PadCodo::ButtonDown)
		{
			return !(GetInstance()->joypadObjs[padIndex].padInput.rgdwPOV[0] == 18000) &&
				(GetInstance()->joypadObjs[padIndex].prevPadInput.rgdwPOV[0] == 18000);
		}
		else
		{
			return !(GetInstance()->joypadObjs[padIndex].padInput.rgbButtons[(int)padCode] & 0x80) &&
				(GetInstance()->joypadObjs[padIndex].prevPadInput.rgbButtons[(int)padCode] & 0x80);
		}

		return false;
	}

	static inline Vec2 GetStick(const PadCodo& padCode, const float& Length = 0, const int& padIndex = 0)
	{
		Vec2 stick = 0;
		if (padCode == PadCodo::StickLeft)
		{
			stick.x = (float)GetInstance()->joypadObjs[padIndex].padInput.lX;
			stick.y = (float)GetInstance()->joypadObjs[padIndex].padInput.lY;
		}
		else if (padCode == PadCodo::StickLeft)
		{
			stick.x = (float)GetInstance()->joypadObjs[padIndex].padInput.lRx;
			stick.y = (float)GetInstance()->joypadObjs[padIndex].padInput.lRy;
		}

		if (stick.Length() > Length)
		{
			return stick.Norm() * Length;
		}

		return 0;
	}
	static inline Vec2 GetStickDown(const PadCodo& padCode, const float& Length = 0, const int& padIndex = 0)
	{
		Vec2 stick = 0;
		Vec2 prevStick = 0;
		if (padCode == PadCodo::StickLeft)
		{
			stick.x = (float)GetInstance()->joypadObjs[padIndex].padInput.lX;
			stick.y = (float)GetInstance()->joypadObjs[padIndex].padInput.lY;

			prevStick.x = (float)GetInstance()->joypadObjs[padIndex].prevPadInput.lX;
			prevStick.y = (float)GetInstance()->joypadObjs[padIndex].prevPadInput.lY;
		}
		else if (padCode == PadCodo::StickRight)
		{
			stick.x = (float)GetInstance()->joypadObjs[padIndex].padInput.lRx;
			stick.y = (float)GetInstance()->joypadObjs[padIndex].padInput.lRy;

			prevStick.x = (float)GetInstance()->joypadObjs[padIndex].prevPadInput.lRx;
			prevStick.y = (float)GetInstance()->joypadObjs[padIndex].prevPadInput.lRy;
		}

		if ((stick.Length() > fabsf(Length)) && !(prevStick.Length() > fabsf(Length)))
		{
			return stick.Norm() * Length;
		}

		return 0;
	}
	static inline Vec2 GetStickUp(const PadCodo& padCode, const float& Length = 0, const int& padIndex = 0)
	{
		Vec2 stick = 0;
		Vec2 prevStick = 0;
		if (padCode == PadCodo::StickLeft)
		{
			stick.x = (float)GetInstance()->joypadObjs[padIndex].padInput.lX;
			stick.y = (float)GetInstance()->joypadObjs[padIndex].padInput.lY;

			prevStick.x = (float)GetInstance()->joypadObjs[padIndex].prevPadInput.lX;
			prevStick.y = (float)GetInstance()->joypadObjs[padIndex].prevPadInput.lY;
		}
		else if (padCode == PadCodo::StickRight)
		{
			stick.x = (float)GetInstance()->joypadObjs[padIndex].padInput.lRx;
			stick.y = (float)GetInstance()->joypadObjs[padIndex].padInput.lRy;

			prevStick.x = (float)GetInstance()->joypadObjs[padIndex].prevPadInput.lRx;
			prevStick.y = (float)GetInstance()->joypadObjs[padIndex].prevPadInput.lRy;
		}

		if (!(stick.Length() > fabsf(Length)) && (prevStick.Length() > fabsf(Length)))
		{
			return stick.Norm() * Length;
		}

		return 0;
	}

	static inline float GetTrigger(const PadCodo& padCode, const float& num = 0, const int& padIndex = 0)
	{
		if (padCode == PadCodo::TriggerLeft)
		{
			if ((float)GetInstance()->joypadObjs[padIndex].padInput.lZ > num)
			{
				return (float)GetInstance()->joypadObjs[padIndex].padInput.lZ;
			}
		}
		else if (padCode == PadCodo::TriggerRight)
		{
			if ((float)GetInstance()->joypadObjs[padIndex].padInput.lZ < -num)
			{
				return (float)GetInstance()->joypadObjs[padIndex].padInput.lZ * -1;
			}
		}

		return 0;
	}
	static inline bool GetTriggerDown(const PadCodo& padCode, const float& num = 0, const int& padIndex = 0)
	{
		if (padCode == PadCodo::TriggerLeft)
		{
			if (((float)GetInstance()->joypadObjs[padIndex].padInput.lZ > num) &&
				!((float)GetInstance()->joypadObjs[padIndex].prevPadInput.lZ > num))
			{
				return (float)GetInstance()->joypadObjs[padIndex].padInput.lZ;
			}
		}
		else if (padCode == PadCodo::TriggerRight)
		{
			if (((float)GetInstance()->joypadObjs[padIndex].padInput.lZ < -num) &&
				!((float)GetInstance()->joypadObjs[padIndex].prevPadInput.lZ < -num))
			{
				return (float)GetInstance()->joypadObjs[padIndex].padInput.lZ * -1;
			}
		}

		return 0;
	}
	static inline bool GetTriggerUp(const PadCodo& padCode, const float& num = 0, const int& padIndex = 0)
	{
		if (padCode == PadCodo::TriggerLeft)
		{
			if (!((float)GetInstance()->joypadObjs[padIndex].padInput.lZ > num) &&
				((float)GetInstance()->joypadObjs[padIndex].prevPadInput.lZ > num))
			{
				return (float)GetInstance()->joypadObjs[padIndex].padInput.lZ;
			}
		}
		else if (padCode == PadCodo::TriggerRight)
		{
			if (!((float)GetInstance()->joypadObjs[padIndex].padInput.lZ < -num) &&
				((float)GetInstance()->joypadObjs[padIndex].prevPadInput.lZ < -num))
			{
				return (float)GetInstance()->joypadObjs[padIndex].padInput.lZ * -1;
			}
		}

		return 0;
	}

	static inline bool GetisLinkPad(const int& padIndex = 0)
	{
		return GetInstance()->joypadObjs[padIndex].joypad != nullptr;
	}
};

typedef JoypadInput Pad;