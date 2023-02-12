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
	friend Singleton<MouseInput>;
	Microsoft::WRL::ComPtr<IDirectInputDevice8> mouse;
	Vec2 mousePos;
	DIMOUSESTATE2 mouseInput;
	DIMOUSESTATE2 prevMouseInput;

private:
	MouseInput();

public:
	void Init();
	void Update();

	// クリックしてる時
	static inline bool GetClick(const MouseCodo& mouseCodo)
	{
		return (GetInstance()->mouseInput.rgbButtons[(int)mouseCodo] & 0x80) != 0;
	}
	// クリックした瞬間
	static inline bool GetClickDown(const MouseCodo& mouseCodo)
	{
		return (GetInstance()->mouseInput.rgbButtons[(int)mouseCodo] & 0x80) == 0 &&
			(GetInstance()->prevMouseInput.rgbButtons[(int)mouseCodo] & 0x80) != 0;
	}
	// クリックし終わった瞬間
	static inline bool GetClickUp(const MouseCodo& mouseCodo)
	{
		return (GetInstance()->mouseInput.rgbButtons[(int)mouseCodo] & 0x80) != 0 &&
			(GetInstance()->prevMouseInput.rgbButtons[(int)mouseCodo] & 0x80) == 0;
	}
	// マウスの座標
	static inline Vec2 GetMousePos()
	{
		return GetInstance()->mousePos;
	}
	// マウスの動いているベクトル
	static inline Vec2 GetMouseMove()
	{
		return { (float)GetInstance()->mouseInput.lX, (float)GetInstance()->mouseInput.lY };
	}
	// マウスホイルの動いているベクトル
	static inline float GetWheelMove()
	{
		return (float)GetInstance()->mouseInput.lZ;
	}
};

typedef MouseInput Mouse;