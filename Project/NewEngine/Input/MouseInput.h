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

	// クリックしてる時
	static bool GetClick(const MouseCode mouseCode);

	// クリックした瞬間
	static bool GetClickDown(const MouseCode mouseCode);

	// クリックし終わった瞬間
	static bool GetClickUp(const MouseCode mouseCode);

	// マウスの座標
	static Vec2 GetPos();

	// マウスの動いているベクトル
	static Vec2 GetMoveVec();

	// マウスホイルの動いているベクトル
	static float GetWheelMoveVec();

private:
	MouseInput();
	friend Singleton<MouseInput>;
};

typedef MouseInput Mouse;