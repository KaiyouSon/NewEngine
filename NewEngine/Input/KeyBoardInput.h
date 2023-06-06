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
	void Init();	// ������
	void Update();		// �X�V����

	// �L�[��������Ă鎞
	static bool GetKey(const unsigned int key);

	// �L�[���������u��
	static bool GetKeyDown(const unsigned int key);

	// �L�[�𗣂����u��
	static bool GetKeyUp(const unsigned int key);

private:
	friend Singleton<KeyBoardInput>;
	KeyBoardInput();
};

typedef KeyBoardInput Key;