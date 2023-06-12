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
	// �\����
	struct JoypadObj
	{
		Microsoft::WRL::ComPtr<IDirectInputDevice8> joypad;
		DIJOYSTATE2 padInput;
		DIJOYSTATE2 prevPadInput;
	};

private: // �Y�����֘A
	int minButton_;
	int maxButton_;

private:// �{�^���֘A
	std::vector<JoypadObj> joypadObjs_;
	static bool sIsInsertPad_;
	static uint32_t sPadIndex_;

private:
	// �R���g���[���[�ڑ��������̃R�[���o�b�N
	static BOOL CALLBACK DeviceFindCallBack(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext);

	// �R���g���[���[��ڑ����鏈��
	void SetJoyStick();

public:
	void Init();
	void Update();

public: // �{�^���֘A
	static bool GetButton(const PadCode padCode, const int padIndex = 0);
	static bool GetButtonDown(const PadCode padCode, const int padIndex = 0);
	static bool GetButtonUp(const PadCode padCode, const int padIndex = 0);

public: // �X�e�B�b�N�֘A
	static Vec2 GetStick(const PadCode padCode, const int padIndex = 0);
	static Vec2 GetStickDown(const PadCode padCode, const float length = 0, const int padIndex = 0);
	static Vec2 GetStickUp(const PadCode padCode, const float length = 0, const int padIndex = 0);

public:	// �g���K�[�֘A
	static float GetTrigger(const PadCode padCode, const int padIndex = 0);
	static bool GetTriggerDown(const PadCode padCode, const float length = 0, const int padIndex = 0);
	static bool GetTriggerUp(const PadCode padCode, const float length = 0, const int padIndex = 0);

public: // ���̑��擾�֘A
	static bool GetisLinkPad(const int padIndex = 0);

public: // �A�T�b�g
	static inline void SetisInsertPad(const bool isInsertPad) { JoypadInput::sIsInsertPad_ = isInsertPad; }
	static inline bool GetisInsertPad() { return JoypadInput::sIsInsertPad_; }

private:
	friend Singleton<JoypadInput>;
	JoypadInput();
};

typedef JoypadInput Pad;