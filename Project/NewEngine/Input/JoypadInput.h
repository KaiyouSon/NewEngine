#pragma once
#include "MathUtil.h"
#include "Singleton.h"
#include <vector>
#include <dinput.h>
#include <wrl.h>

// パッドコード
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

// 前方宣言
template<typename T> class Singleton;

// コントローラーのクラス
class JoypadInput : public Singleton<JoypadInput>
{
private:
	// ジョイパッド情報
	struct JoypadObj
	{
		Microsoft::WRL::ComPtr<IDirectInputDevice8> joypad;
		DIJOYSTATE2 padInput;
		DIJOYSTATE2 prevPadInput;
	};

private: // メンバ変数
	std::vector<JoypadObj> mJoypadObjs;
	static bool sIsInsertPad;
	static uint32_t sPadIndex;

private: // プライベートメソッド
	// デバイスを探すためのコールバック関数
	static BOOL CALLBACK DeviceFindCallBack(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext);

	// デバイスを見つけて初期化するためのメソッド
	void SetJoyStick();

public:
	void Init();
	void Update();

public: // メンバ関数
	static bool GetButton(const PadCode padCode, const int padIndex = 0);
	static bool GetButtonDown(const PadCode padCode, const int padIndex = 0);
	static bool GetButtonUp(const PadCode padCode, const int padIndex = 0);

	static bool GetAnyButtonDown(const int padIndex = 0);

public: // 入力取得メソッド
	static Vec2 GetStick(const PadCode padCode, const float length = 0, const int padIndex = 0);
	static Vec2 GetStickDown(const PadCode padCode, const float length = 0, const int padIndex = 0);
	static Vec2 GetStickUp(const PadCode padCode, const float length = 0, const int padIndex = 0);
	static Vec3 GetStickVec3(const PadCode padCode, const float length = 0, const int padIndex = 0);

public: // トリガー取得メソッド
	static float GetTrigger(const PadCode padCode, const float length = 0, const int padIndex = 0);
	static bool GetTriggerDown(const PadCode padCode, const float length = 0, const int padIndex = 0);
	static bool GetTriggerUp(const PadCode padCode, const float length = 0, const int padIndex = 0);

public: // リンクパッド情報取得
	static bool GetisLinkPad();

public:	// その他

	// 挿入フラグ
	static void SetisInsertPad(const bool isInsertPad);
	static bool GetisInsertPad();

private:
	friend Singleton<JoypadInput>;
	JoypadInput();
};

typedef JoypadInput Pad;
