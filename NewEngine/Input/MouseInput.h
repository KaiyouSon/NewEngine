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

	// クリックしてる時
	static bool GetClick(const MouseCodo mouseCodo);

	// クリックした瞬間
	static bool GetClickDown(const MouseCodo mouseCodo);

	// クリックし終わった瞬間
	static bool GetClickUp(const MouseCodo mouseCodo);

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