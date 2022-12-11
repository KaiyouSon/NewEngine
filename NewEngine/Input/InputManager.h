#pragma once
#define DIRECTINPUT_VERSION 0x0800	// DirectInputのバージョン指定
#include "JoypadInput.h"
#include "KeyBoardInput.h"
#include "MouseInput.h"
#include <dinput.h>
#include <wrl.h>

enum ButtonCodo
{
	A = 0,
	B = 1,
	X = 2,
	Y = 3,
	L1 = 4,
	R1 = 5,
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