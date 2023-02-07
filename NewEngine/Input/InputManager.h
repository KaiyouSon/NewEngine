#pragma once
#define DIRECTINPUT_VERSION 0x0800	// DirectInput�̃o�[�W�����w��
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
	Microsoft::WRL::ComPtr<IDirectInput8> directInput;

public:
	void Init();
	void Update();

public: // �Q�b�^�[
	inline Microsoft::WRL::ComPtr<IDirectInput8> GetDirectInput() { return directInput; }
};