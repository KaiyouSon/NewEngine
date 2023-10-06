#pragma once
#define DIRECTINPUT_VERSION 0x0800	// DirectInput縺ｮ繝舌・繧ｸ繝ｧ繝ｳ謖・ｮ・
#include "JoypadInput.h"
#include "KeyBoardInput.h"
#include "MouseInput.h"
#include <dinput.h>
#include <wrl.h>

template<typename T> class Singleton;

class InputManager : public Singleton<InputManager>
{
private:
	friend Singleton<InputManager>;
	Microsoft::WRL::ComPtr<IDirectInput8> mDirectInput;

public:
	void Init();
	void Update();

public: // 繧ｲ繝・ち繝ｼ
	IDirectInput8* GetDirectInput();
};
