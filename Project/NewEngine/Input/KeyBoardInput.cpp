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

	// �L�[�{�[�h�f�o�C�X�̐���
	result = InputManager::GetInstance()->GetDirectInput()->
		CreateDevice(GUID_SysKeyboard, &keyboard_, nullptr);
	assert(SUCCEEDED(result));

	// ���̓f�[�^�`���̃Z�b�g
	result = keyboard_->SetDataFormat(&c_dfDIKeyboard);	// �W���`��
	assert(SUCCEEDED(result));

	// �r�����䃌�x���̃Z�b�g
	result = keyboard_->SetCooperativeLevel(
		renderWindow->GetHwnd(),
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}
void KeyBoardInput::Update()
{
	// �L�[�{�[�g���̎擾�J�n
	keyboard_->Acquire();

	// �ŐV�̃L�[�{�[�h��񂾂������̂�1�t���[���O�̃L�[�{�[�h���Ƃ��ĕۑ�
	for (uint32_t i = 0; i < 256; ++i)
	{
		prevKeys_[i] = keys_[i];
	}

	// �ŐV�̃L�[�{�[�h�����擾����
	keyboard_->GetDeviceState(sizeof(keys_), keys_.data());
}

// �L�[��������Ă鎞
bool KeyBoardInput::GetKey(const uint32_t key)
{
	// ��O�X���[���Ȃ��悤��
	int index = Clamp<int>(key, 0, 256);

	return (bool)GetInstance()->keys_[index];
}

// �L�[���������u��
bool KeyBoardInput::GetKeyDown(const uint32_t key)
{
	// ��O�X���[���Ȃ��悤��
	int index = Clamp<int>(key, 0, 256);

	return (bool)(GetInstance()->keys_[key] && !GetInstance()->prevKeys_[key]);
}

// �L�[�𗣂����u��
bool KeyBoardInput::GetKeyUp(const uint32_t key)
{
	// ��O�X���[���Ȃ��悤��
	int index = Clamp<int>(key, 0, 256);

	return (bool)(!GetInstance()->keys_[key] && GetInstance()->prevKeys_[key]);
}
