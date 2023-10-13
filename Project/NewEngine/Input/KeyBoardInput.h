#pragma once
#include "Singleton.h"
#include <dinput.h>
#include <wrl.h>
#include <array>

template<typename T> class Singleton;

class KeyBoardInput : public Singleton<KeyBoardInput>
{
private:
	Microsoft::WRL::ComPtr<IDirectInputDevice8> mKeyboard;

	std::array<unsigned char, 256> mKeys;
	std::array<unsigned char, 256> mPrevKeys;

public:
	void Init();	// 蛻晄悄蛹・
	void Update();	// 譖ｴ譁ｰ蜃ｦ逅・

	// 繧ｭ繝ｼ縺梧款縺輔ｌ縺ｦ繧区凾
	static bool GetKey(const uint32_t key);

	// 繧ｭ繝ｼ繧呈款縺励◆迸ｬ髢・
	static bool GetKeyDown(const uint32_t key);

	// 繧ｭ繝ｼ繧帝屬縺励◆迸ｬ髢・
	static bool GetKeyUp(const uint32_t key);

private:
	friend Singleton<KeyBoardInput>;
	KeyBoardInput();
};

typedef KeyBoardInput Key;
