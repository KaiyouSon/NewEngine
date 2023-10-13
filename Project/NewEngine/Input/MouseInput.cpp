#include "InputManager.h"
#include "MouseInput.h"
#include "RenderWindow.h"
#include <cassert>

MouseInput::MouseInput() :
	mMouse(nullptr), mMouseInput({}), mPrevMouseInput({})
{
}
void MouseInput::Init()
{
	HRESULT result;
	RenderWindow* renderWindow = RenderWindow::GetInstance().get();

	// 繝槭え繧ｹ繝・ヰ繧､繧ｹ縺ｮ逕滓・
	result = InputManager::GetInstance()->GetDirectInput()->
		CreateDevice(GUID_SysMouse, &mMouse, nullptr);
	assert(SUCCEEDED(result));

	// 蜈･蜉帙ョ繝ｼ繧ｿ蠖｢蠑上・繧ｻ繝・ヨ
	result = mMouse->SetDataFormat(&c_dfDIMouse2); // 諡｡蠑ｵ8繝懊ち繝ｳ縺ｾ縺ｧ
	assert(SUCCEEDED(result));

	// 謗剃ｻ門宛蠕｡繝ｬ繝吶Ν縺ｮ繧ｻ繝・ヨ
	result = mMouse->SetCooperativeLevel(
		renderWindow->GetHwnd(),
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	assert(SUCCEEDED(result));
}
void MouseInput::Update()
{
	RenderWindow* renderWindow = RenderWindow::GetInstance().get();

	// 繝槭え繧ｹ諠・ｱ縺ｮ蜿門ｾ鈴幕蟋・
	mMouse->Acquire();

	// 譛譁ｰ縺ｮ繝槭え繧ｹ諠・ｱ縺縺｣縺溘ｂ縺ｮ縺ｯ1繝輔Ξ繝ｼ繝蜑阪・繧ｭ繝ｼ繝懊・繝画ュ蝣ｱ縺ｨ縺励※菫晏ｭ・
	mPrevMouseInput = mMouseInput;

	// 譛譁ｰ縺ｮ繝槭え繧ｹ諠・ｱ繧貞叙蠕励☆繧・
	mMouse->GetDeviceState(sizeof(mMouseInput), &mMouseInput);

	POINT tempMousePos;
	GetCursorPos(&tempMousePos);

	// 繧ｦ繧｣繝ｳ繝峨え蠎ｧ讓吶↓螟画鋤縺吶ｋ
	ScreenToClient(renderWindow->GetHwnd(), &tempMousePos);
	mMousePos.x = static_cast<float>(tempMousePos.x);
	mMousePos.y = static_cast<float>(tempMousePos.y);
}

// 繧ｯ繝ｪ繝・け縺励※縺・ｋ髢・
bool MouseInput::GetClick(const MouseCode mouseCode)
{
	bool isClick = (GetInstance()->mMouseInput.rgbButtons[(int)mouseCode] & 0x80) != 0;

	return isClick;
}

// 繧ｯ繝ｪ繝・け縺励◆迸ｬ髢・
bool MouseInput::GetClickDown(const MouseCode mouseCode)
{
	bool isClick = (GetInstance()->mMouseInput.rgbButtons[(int)mouseCode] & 0x80) != 0;
	bool isPrevClick = (GetInstance()->mPrevMouseInput.rgbButtons[(int)mouseCode] & 0x80) != 0;

	return isClick && !isPrevClick;
}

// 繧ｯ繝ｪ繝・け縺礼ｵゅｏ縺｣縺溽椪髢・
bool MouseInput::GetClickUp(const MouseCode mouseCode)
{
	bool isClick = (GetInstance()->mMouseInput.rgbButtons[(int)mouseCode] & 0x80) != 0;
	bool isPrevClick = (GetInstance()->mPrevMouseInput.rgbButtons[(int)mouseCode] & 0x80) != 0;

	return !isClick && isPrevClick;
}

// 繝槭え繧ｹ縺ｮ蠎ｧ讓・
Vec2 MouseInput::GetPos()
{
	return GetInstance()->mMousePos;
}

// 繝槭え繧ｹ縺ｮ蜍輔＞縺ｦ縺・ｋ繝吶け繝医Ν
Vec2 MouseInput::GetMoveVec()
{
	return { (float)GetInstance()->mMouseInput.lX, (float)GetInstance()->mMouseInput.lY };
}

// 繝槭え繧ｹ繝帙う繝ｫ縺ｮ蜍輔＞縺ｦ縺・ｋ繝吶け繝医Ν
float MouseInput::GetWheelMoveVec()
{
	return (float)GetInstance()->mMouseInput.lZ;
}

