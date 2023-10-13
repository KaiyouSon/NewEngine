#include "InputManager.h"
#include "KeyBoardInput.h"
#include "RenderWindow.h"
#include <cassert>
#include <memory>

KeyBoardInput::KeyBoardInput()
{
}
void KeyBoardInput::Init()
{
	HRESULT result;
	RenderWindow* renderWindow = RenderWindow::GetInstance().get();

	// 繧ｭ繝ｼ繝懊・繝峨ョ繝舌う繧ｹ縺ｮ逕滓・
	result = InputManager::GetInstance()->GetDirectInput()->
		CreateDevice(GUID_SysKeyboard, &mKeyboard, nullptr);
	assert(SUCCEEDED(result));

	// 蜈･蜉帙ョ繝ｼ繧ｿ蠖｢蠑上・繧ｻ繝・ヨ
	result = mKeyboard->SetDataFormat(&c_dfDIKeyboard);	// 讓呎ｺ門ｽ｢蠑・
	assert(SUCCEEDED(result));

	// 謗剃ｻ門宛蠕｡繝ｬ繝吶Ν縺ｮ繧ｻ繝・ヨ
	result = mKeyboard->SetCooperativeLevel(
		renderWindow->GetHwnd(),
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}
void KeyBoardInput::Update()
{
	// 繧ｭ繝ｼ繝懊・繝域ュ蝣ｱ縺ｮ蜿門ｾ鈴幕蟋・
	mKeyboard->Acquire();

	// 譛譁ｰ縺ｮ繧ｭ繝ｼ繝懊・繝画ュ蝣ｱ縺縺｣縺溘ｂ縺ｮ縺ｯ1繝輔Ξ繝ｼ繝蜑阪・繧ｭ繝ｼ繝懊・繝画ュ蝣ｱ縺ｨ縺励※菫晏ｭ・
	for (uint32_t i = 0; i < 256; ++i)
	{
		mPrevKeys[i] = mKeys[i];
	}

	// 譛譁ｰ縺ｮ繧ｭ繝ｼ繝懊・繝画ュ蝣ｱ繧貞叙蠕励☆繧・
	mKeyboard->GetDeviceState(sizeof(mKeys), mKeys.data());
}

// 繧ｭ繝ｼ縺梧款縺輔ｌ縺ｦ繧区凾
bool KeyBoardInput::GetKey(const uint32_t key)
{
	// 萓句､悶せ繝ｭ繝ｼ縺励↑縺・ｈ縺・↓
	uint32_t index = Clamp<uint32_t>(key, 0, 256);

	return (bool)GetInstance()->mKeys[index];
}

// 繧ｭ繝ｼ繧呈款縺励◆迸ｬ髢・
bool KeyBoardInput::GetKeyDown(const uint32_t key)
{
	// 萓句､悶せ繝ｭ繝ｼ縺励↑縺・ｈ縺・↓
	uint32_t index = Clamp<uint32_t>(key, 0, 256);

	return (bool)(GetInstance()->mKeys[index] && !GetInstance()->mPrevKeys[index]);
}

// 繧ｭ繝ｼ繧帝屬縺励◆迸ｬ髢・
bool KeyBoardInput::GetKeyUp(const uint32_t key)
{
	// 萓句､悶せ繝ｭ繝ｼ縺励↑縺・ｈ縺・↓
	uint32_t index = Clamp<uint32_t>(key, 0, 256);

	return (bool)(!GetInstance()->mKeys[index] && GetInstance()->mPrevKeys[index]);
}

