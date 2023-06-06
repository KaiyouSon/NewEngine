#pragma once
#include "Singleton.h"
#include <dinput.h>
#include <wrl.h>

template<typename T> class Singleton;

class KeyBoardInput : public Singleton<KeyBoardInput>
{
private:
	Microsoft::WRL::ComPtr<IDirectInputDevice8> keyboard;
	BYTE keys[256] = { 0 };
	BYTE prevKeys[256] = { 0 };
public:
	void Init();	// 初期化
	void Update();		// 更新処理

	// キーが押されてる時
	static bool GetKey(const unsigned int key);

	// キーを押した瞬間
	static bool GetKeyDown(const unsigned int key);

	// キーを離した瞬間
	static bool GetKeyUp(const unsigned int key);

private:
	friend Singleton<KeyBoardInput>;
	KeyBoardInput();
};

typedef KeyBoardInput Key;