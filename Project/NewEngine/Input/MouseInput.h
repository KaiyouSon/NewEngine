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
	Microsoft::WRL::ComPtr<IDirectInputDevice8> mMouse;
	Vec2 mMousePos;
	DIMOUSESTATE2 mMouseInput;
	DIMOUSESTATE2 mPrevMouseInput;

public:
	void Init();
	void Update();

	// 繧ｯ繝ｪ繝・け縺励※繧区凾
	static bool GetClick(const MouseCode mouseCode);

	// 繧ｯ繝ｪ繝・け縺励◆迸ｬ髢・
	static bool GetClickDown(const MouseCode mouseCode);

	// 繧ｯ繝ｪ繝・け縺礼ｵゅｏ縺｣縺溽椪髢・
	static bool GetClickUp(const MouseCode mouseCode);

	// 繝槭え繧ｹ縺ｮ蠎ｧ讓・
	static Vec2 GetPos();

	// 繝槭え繧ｹ縺ｮ蜍輔＞縺ｦ縺・ｋ繝吶け繝医Ν
	static Vec2 GetMoveVec();

	// 繝槭え繧ｹ繝帙う繝ｫ縺ｮ蜍輔＞縺ｦ縺・ｋ繝吶け繝医Ν
	static float GetWheelMoveVec();

private:
	MouseInput();
	friend Singleton<MouseInput>;
};

typedef MouseInput Mouse;
