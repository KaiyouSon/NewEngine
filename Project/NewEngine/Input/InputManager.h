#pragma once
#define DIRECTINPUT_VERSION 0x0800	// DirectInputバージョン指定
#include "JoypadInput.h"
#include "KeyBoardInput.h"
#include "MouseInput.h"
#include <dinput.h>
#include <wrl.h>

// 前方宣言
template<typename T> class Singleton;

// 入力関連を管理するクラス
class InputManager : public Singleton<InputManager>
{
private:
	Microsoft::WRL::ComPtr<IDirectInput8> mDirectInput;

public:
	void Init();
	void Update();

public: // ゲッター
	IDirectInput8* GetDirectInput();

private:
	InputManager() {}
	friend Singleton<InputManager>;
};