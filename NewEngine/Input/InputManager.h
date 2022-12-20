#pragma once
#define DIRECTINPUT_VERSION 0x0800	// DirectInputのバージョン指定
#include "JoypadInput.h"
#include "KeyBoardInput.h"
#include "MouseInput.h"
#include <dinput.h>
#include <wrl.h>

enum PadCodo
{
	ButtonA = 0,
	ButtonB = 1,
	ButtonX = 2,
	ButtonY = 3,
	ButtonL1 = 4,
	ButtonR1 = 5,

	LeftStick = 96,
	RightStick = 97,
	LeftTrigger = 98,
	RightTrigger = 99,
};

enum MouseCodo
{
	Left = 0,
	Right = 1,
	Wheel = 2,
};

template<typename T> class Singleton;

class InputManager : public Singleton<InputManager>
{
private:
	friend Singleton<InputManager>;
	Microsoft::WRL::ComPtr<IDirectInput8> directInput;

public:
	void Init();
	void Update();

public: // ゲッター
	inline Microsoft::WRL::ComPtr<IDirectInput8> GetDirectInput() { return directInput; }
};