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
	DIJOYSTATE2 oldPadInput;

	static BOOL CALLBACK DeviceFindCallBack(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext);
public:
	void Initialize();
	void Update();

	bool GetButton(const int& buttonNumber);
	bool GetButtonTrigger(const int& buttonNumber);
	bool GetButtonReleased(const int& buttonNumber);
	Vec2 GetLeftStickParam();
	Vec2 GetLeftStickVec();
	Vec2 GetRightStickParam();
	Vec2 GetRightStickVec();
};