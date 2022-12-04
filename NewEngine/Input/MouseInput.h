#pragma once
#include "MathUtil.h"
#include "Singleton.h"
#include <dinput.h>
#include <wrl.h>

template<typename T> class Singleton;

class MouseInput : public Singleton<MouseInput>
{
private:
	friend Singleton<MouseInput>;
	Microsoft::WRL::ComPtr<IDirectInputDevice8> mouse;
	Vec2 mousePos;
	DIMOUSESTATE2 mouseInput;
	DIMOUSESTATE2 prevMouseInput;
public:
	void Initialize();
	void Update();

	// �}�E�X�̎擾�֘A

	// �N���b�N���Ă鎞
	static inline bool GetClick(const unsigned int& mouse)
	{
		return (GetInstance()->mouseInput.rgbButtons[mouse] & 0x80) != 0;
	}
	// �N���b�N�����u��
	static inline bool GetClickTrigger(const unsigned int& mouse)
	{
		return (GetInstance()->mouseInput.rgbButtons[mouse] & 0x80) == 0 &&
			(GetInstance()->prevMouseInput.rgbButtons[mouse] & 0x80) != 0;
	}
	// �N���b�N���I������u��
	static inline bool GetClickReleased(const unsigned int& mouse)
	{
		return (GetInstance()->mouseInput.rgbButtons[mouse] & 0x80) != 0 &&
			(GetInstance()->prevMouseInput.rgbButtons[mouse] & 0x80) == 0;
	}
	// �}�E�X�̍��W
	static inline Vec2 GetMousePos()
	{
		return GetInstance()->mousePos;
	}
	// �}�E�X�̓����Ă���x�N�g��
	static inline Vec2 GetMouseMove()
	{
		return { (float)GetInstance()->mouseInput.lX, (float)GetInstance()->mouseInput.lY };
	}
	// �}�E�X�z�C���̓����Ă���x�N�g��
	static inline float GetWheelMove()
	{
		return (float)GetInstance()->mouseInput.lZ;
	}
};

typedef MouseInput Mouse;