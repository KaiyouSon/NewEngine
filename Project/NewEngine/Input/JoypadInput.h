#pragma once
#include "MathUtil.h"
#include "Singleton.h"
#include <vector>
#include <dinput.h>
#include <wrl.h>

enum class PadCode
{
	ButtonA = 0,
	ButtonB = 1,
	ButtonX = 2,
	ButtonY = 3,
	ButtonL1 = 4,
	ButtonR1 = 5,
	ButtonStart = 7,

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
	// 讒矩菴・
	struct JoypadObj
	{
		Microsoft::WRL::ComPtr<IDirectInputDevice8> joypad;
		DIJOYSTATE2 padInput;
		DIJOYSTATE2 prevPadInput;
	};

private:// 繝懊ち繝ｳ髢｢騾｣
	std::vector<JoypadObj> mJoypadObjs;
	static bool sIsInsertPad;
	static uint32_t sPadIndex;

private:
	// 繧ｳ繝ｳ繝医Ο繝ｼ繝ｩ繝ｼ謗･邯壹＠縺滓凾縺ｮ繧ｳ繝ｼ繝ｫ繝舌ャ繧ｯ
	static BOOL CALLBACK DeviceFindCallBack(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext);

	// 繧ｳ繝ｳ繝医Ο繝ｼ繝ｩ繝ｼ繧呈磁邯壹☆繧句・逅・
	void SetJoyStick();

public:
	void Init();
	void Update();

public: // 繝懊ち繝ｳ髢｢騾｣
	static bool GetButton(const PadCode padCode, const int padIndex = 0);
	static bool GetButtonDown(const PadCode padCode, const int padIndex = 0);
	static bool GetButtonUp(const PadCode padCode, const int padIndex = 0);

	static bool GetAnyButtonDown(const int padIndex = 0);

public: // 繧ｹ繝・ぅ繝・け髢｢騾｣
	static Vec2 GetStick(const PadCode padCode, const float length = 0, const int padIndex = 0);
	static Vec2 GetStickDown(const PadCode padCode, const float length = 0, const int padIndex = 0);
	static Vec2 GetStickUp(const PadCode padCode, const float length = 0, const int padIndex = 0);
	static Vec3 GetStickVec3(const PadCode padCode, const float length = 0, const int padIndex = 0);


public:	// 繝医Μ繧ｬ繝ｼ髢｢騾｣
	static float GetTrigger(const PadCode padCode, const float length = 0, const int padIndex = 0);
	static bool GetTriggerDown(const PadCode padCode, const float length = 0, const int padIndex = 0);
	static bool GetTriggerUp(const PadCode padCode, const float length = 0, const int padIndex = 0);

public: // 縺昴・莉門叙蠕鈴未騾｣
	static bool GetisLinkPad();

public: // 繧｢繧ｵ繝・ヨ
	static inline void SetisInsertPad(const bool isInsertPad) { JoypadInput::sIsInsertPad = isInsertPad; }
	static inline bool GetisInsertPad() { return JoypadInput::sIsInsertPad; }

private:
	friend Singleton<JoypadInput>;
	JoypadInput();
};

typedef JoypadInput Pad;
