#include "InputManager.h"
#include "KeyBoardInput.h"
#include "RenderWindow.h"
#include <cassert>
#include <memory>

#pragma region ���̑��̏���

KeyBoardInput::KeyBoardInput()
{
}

void KeyBoardInput::Init()
{
	HRESULT result;
	RenderWindow* renderWindow = RenderWindow::GetInstance().get();

	// �L�[�{�[�h�f�o�C�X�̐���
	result = InputManager::GetInstance()->GetDirectInput()->
		CreateDevice(GUID_SysKeyboard, &keyboard, nullptr);
	assert(SUCCEEDED(result));

	// ���̓f�[�^�`���̃Z�b�g
	result = keyboard->SetDataFormat(&c_dfDIKeyboard);	// �W���`��
	assert(SUCCEEDED(result));

	// �r�����䃌�x���̃Z�b�g
	result = keyboard->SetCooperativeLevel(
		renderWindow->GetHwnd(),
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}

void KeyBoardInput::Update()
{
	// �L�[�{�[�g���̎擾�J�n
	keyboard->Acquire();

	// �ŐV�̃L�[�{�[�h��񂾂������̂�1�t���[���O�̃L�[�{�[�h���Ƃ��ĕۑ�
	for (uint32_t i = 0; i < 256; ++i)
	{
		prevKeys[i] = keys[i];
	}

	// �ŐV�̃L�[�{�[�h�����擾����
	keyboard->GetDeviceState(sizeof(keys), keys);
}

#pragma endregion

#pragma region �L�[�{�[�h�̏���

// �L�[��������Ă鎞
bool KeyBoardInput::GetKey(const unsigned int key)
{
	// ��O�X���[���Ȃ��悤��
	int index = Clamp(key, 0, 256);

	return (bool)GetInstance()->keys[index];
}

// �L�[���������u��
bool KeyBoardInput::GetKeyDown(const unsigned int key)
{
	// ��O�X���[���Ȃ��悤��
	int index = Clamp(key, 0, 256);

	return (bool)(GetInstance()->keys[key] && !GetInstance()->prevKeys[key]);
}

// �L�[�𗣂����u��
bool KeyBoardInput::GetKeyUp(const unsigned int key)
{
	// ��O�X���[���Ȃ��悤��
	int index = Clamp(key, 0, 256);

	return (bool)(!GetInstance()->keys[key] && GetInstance()->prevKeys[key]);
}

#pragma endregion
