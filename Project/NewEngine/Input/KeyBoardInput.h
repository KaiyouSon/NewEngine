#pragma once
#include "Singleton.h"
#include <dinput.h>
#include <wrl.h>
#include <array>

template<typename T> class Singleton;

class KeyBoardInput : public Singleton<KeyBoardInput>
{
private:
	Microsoft::WRL::ComPtr<IDirectInputDevice8> keyboard_;

	std::array<unsigned char, 256> keys_;
	std::array<unsigned char, 256> prevKeys_;

public:
	void Init();	// 初期化
	void Update();	// 更新処理

	// キーが押されてる時
	static bool GetKey(const uint32_t key);

	// キーを押した瞬間
	static bool GetKeyDown(const uint32_t key);

	// キーを離した瞬間
	static bool GetKeyUp(const uint32_t key);

private:
	friend Singleton<KeyBoardInput>;
	KeyBoardInput();
};

typedef KeyBoardInput Key;