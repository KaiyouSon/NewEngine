#pragma once
#include "Singleton.h"
#include <dinput.h>
#include <wrl.h>

template<typename T> class Singleton;

class KeyBoardInput : public Singleton<KeyBoardInput>
{
private:
	friend Singleton<KeyBoardInput>;
	Microsoft::WRL::ComPtr<IDirectInputDevice8> keyboard;
	BYTE keys[256] = { 0 };
	BYTE prevKeys[256] = { 0 };
public:
	void Initialize();	// 初期化
	void Update();		// 更新処理

	// キーが押されてる時
	static inline bool GetKey(const unsigned int& key)
	{
		if (key >= 256 || key < 0) return false;
		return (bool)GetInstance()->keys[key];
	}
	// キーを押した瞬間
	static inline bool GetKeyTrigger(const unsigned int& key)
	{
		if (key >= 256 || key < 0) return false;
		return (bool)(!GetInstance()->keys[key] && GetInstance()->prevKeys[key]);
	}
	// キーを離した瞬間
	inline bool GetKeyReleased(const unsigned int& key)
	{
		if (key >= 256 || key < 0) return false;
		return (bool)(GetInstance()->keys[key] && !GetInstance()->prevKeys[key]);
	}
};

typedef KeyBoardInput Key;