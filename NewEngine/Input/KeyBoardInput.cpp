#include "InputManager.h"
#include "KeyBoardInput.h"
#include "RenderWindow.h"
#include <cassert>
#include <memory>

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
