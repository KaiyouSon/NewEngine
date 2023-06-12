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
	// 構造体
	struct JoypadObj
	{
		Microsoft::WRL::ComPtr<IDirectInputDevice8> joypad;
		DIJOYSTATE2 padInput;
		DIJOYSTATE2 prevPadInput;
	};

private: // 添え字関連
	int minButton_;
	int maxButton_;

private:// ボタン関連
	std::vector<JoypadObj> joypadObjs_;
	static bool sIsInsertPad_;
	static uint32_t sPadIndex_;

private:
	// コントローラー接続した時のコールバック
	static BOOL CALLBACK DeviceFindCallBack(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext);

	// コントローラーを接続する処理
	void SetJoyStick();

public:
	void Init();
	void Update();

public: // ボタン関連
	static bool GetButton(const PadCode padCode, const int padIndex = 0);
	static bool GetButtonDown(const PadCode padCode, const int padIndex = 0);
	static bool GetButtonUp(const PadCode padCode, const int padIndex = 0);

public: // スティック関連
	static Vec2 GetStick(const PadCode padCode, const int padIndex = 0);
	static Vec2 GetStickDown(const PadCode padCode, const float length = 0, const int padIndex = 0);
	static Vec2 GetStickUp(const PadCode padCode, const float length = 0, const int padIndex = 0);

public:	// トリガー関連
	static float GetTrigger(const PadCode padCode, const int padIndex = 0);
	static bool GetTriggerDown(const PadCode padCode, const float length = 0, const int padIndex = 0);
	static bool GetTriggerUp(const PadCode padCode, const float length = 0, const int padIndex = 0);

public: // その他取得関連
	static bool GetisLinkPad(const int padIndex = 0);

public: // アサット
	static inline void SetisInsertPad(const bool isInsertPad) { JoypadInput::sIsInsertPad_ = isInsertPad; }
	static inline bool GetisInsertPad() { return JoypadInput::sIsInsertPad_; }

private:
	friend Singleton<JoypadInput>;
	JoypadInput();
};

typedef JoypadInput Pad;