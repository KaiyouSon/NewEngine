#pragma once
#include "Singleton.h"
#include <dinput.h>
#include <wrl.h>
#include <array>

// 前方宣言
template<typename T> class Singleton;

// キーのクラス
class KeyBoardInput : public Singleton<KeyBoardInput>
{
private:
	Microsoft::WRL::ComPtr<IDirectInputDevice8> mKeyboard;

	std::array<unsigned char, 256> mKeys;     // キーボードの状態を格納
	std::array<unsigned char, 256> mPrevKeys; // 前回のキーボードの状態を格納

public:
	void Init();    // 初期化処理
	void Update();  // 入力の更新

	// キーが押されているかを取得
	static bool GetKey(const uint32_t key);

	// キーが押された瞬間の状態を取得
	static bool GetKeyDown(const uint32_t key);

	// キーが離された瞬間の状態を取得
	static bool GetKeyUp(const uint32_t key);

private:
	friend Singleton<KeyBoardInput>;
	KeyBoardInput() {}
};

typedef KeyBoardInput Key;
