#pragma once
#include "MathUtil.h"
#include "Singleton.h"
#include "Enum.h"
#include <dinput.h>
#include <wrl.h>

template<typename T> class Singleton;

class MouseInput : public Singleton<MouseInput>
{
private:
	Microsoft::WRL::ComPtr<IDirectInputDevice8> mouse_;
	Vec2 mousePos_;
	DIMOUSESTATE2 mouseInput_;
	DIMOUSESTATE2 prevMouseInput_;

public:
	void Init();
	void Update();

	// �N���b�N���Ă鎞
	static bool GetClick(const MouseCode mouseCode);

	// �N���b�N�����u��
	static bool GetClickDown(const MouseCode mouseCode);

	// �N���b�N���I������u��
	static bool GetClickUp(const MouseCode mouseCode);

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