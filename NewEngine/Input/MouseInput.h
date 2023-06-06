#pragma once
#include "MathUtil.h"
#include "Singleton.h"
#include <dinput.h>
#include <wrl.h>

enum class MouseCodo
{
	Left = 0,
	Right = 1,
	Wheel = 2,
};

template<typename T> class Singleton;

class MouseInput : public Singleton<MouseInput>
{
private:
	Microsoft::WRL::ComPtr<IDirectInputDevice8> mouse;
	Vec2 mousePos;
	DIMOUSESTATE2 mouseInput;
	DIMOUSESTATE2 prevMouseInput;

public:
	void Init();
	void Update();

	// �N���b�N���Ă鎞
	static bool GetClick(const MouseCodo mouseCodo);

	// �N���b�N�����u��
	static bool GetClickDown(const MouseCodo mouseCodo);

	// �N���b�N���I������u��
	static bool GetClickUp(const MouseCodo mouseCodo);

	// �}�E�X�̍��W
	static Vec2 GetPos();

	// �}�E�X�̓����Ă���x�N�g��
	static Vec2 GetMoveVec();

	// �}�E�X�z�C���̓����Ă���x�N�g��
	static float GetWheelMoveVec();

private:
	MouseInput();
	friend Singleton<MouseInput>;
};

typedef MouseInput Mouse;