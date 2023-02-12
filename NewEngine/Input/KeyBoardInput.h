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
	void Init();	// ������
	void Update();		// �X�V����

	// �L�[��������Ă鎞
	static inline bool GetKey(const unsigned int& key)
	{
		if (key >= 256 || key < 0) return false;
		return (bool)GetInstance()->keys[key];
	}
	// �L�[���������u��
	static inline bool GetKeyDown(const unsigned int& key)
	{
		if (key >= 256 || key < 0) return false;
		return (bool)(GetInstance()->keys[key] && !GetInstance()->prevKeys[key]);
	}
	// �L�[�𗣂����u��
	static inline bool GetKeyUp(const unsigned int& key)
	{
		if (key >= 256 || key < 0) return false;
		return (bool)(!GetInstance()->keys[key] && GetInstance()->prevKeys[key]);
	}
};

typedef KeyBoardInput Key;