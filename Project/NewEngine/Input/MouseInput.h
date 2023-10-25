#pragma once
#include "MathUtil.h"
#include "Singleton.h"
#include "NewEngineEnum.h"
#include <dinput.h>
#include <wrl.h>

// 前方宣言
template<typename T> class Singleton;

// マウスのクラス
class MouseInput : public Singleton<MouseInput>
{
private:
	Microsoft::WRL::ComPtr<IDirectInputDevice8> mMouse;
	Vec2 mMousePos;
	DIMOUSESTATE2 mMouseInput;
	DIMOUSESTATE2 mPrevMouseInput;

public:
	void Init(); // マウスの初期化
	void Update(); // マウスの更新

	// クリックされたかを取得
	static bool GetClick(const MouseCode mouseCode);

	// クリックが開始した瞬間かを取得
	static bool GetClickDown(const MouseCode mouseCode);

	// クリックが終了した瞬間かを取得
	static bool GetClickUp(const MouseCode mouseCode);

	// マウスの位置を取得
	static Vec2 GetPos();

	// マウスの移動ベクトルを取得
	static Vec2 GetMoveVec();

	// マウスホイールの移動量を取得
	static float GetWheelMoveVec();

private:
	MouseInput();
	friend Singleton<MouseInput>;
};

typedef MouseInput Mouse;
