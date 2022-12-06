#include "InputManager.h"
#include "MouseInput.h"
#include "RenderWindow.h"
#include <cassert>

MouseInput::MouseInput() : mouse(nullptr), mouseInput({}), prevMouseInput({})
{
}

void MouseInput::Init()
{
	HRESULT result;
	RenderWindow* renderWindow = RenderWindow::GetInstance().get();

	// �}�E�X�f�o�C�X�̐���
	result = InputManager::GetInstance()->GetDirectInput()->
		CreateDevice(GUID_SysMouse, &mouse, nullptr);
	assert(SUCCEEDED(result));

	// ���̓f�[�^�`���̃Z�b�g
	result = mouse->SetDataFormat(&c_dfDIMouse2); // �g��8�{�^���܂�
	assert(SUCCEEDED(result));

	// �r�����䃌�x���̃Z�b�g
	result = mouse->SetCooperativeLevel(
		renderWindow->GetHwnd(),
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	assert(SUCCEEDED(result));
}

void MouseInput::Update()
{
	RenderWindow* renderWindow = RenderWindow::GetInstance().get();

	// �}�E�X���̎擾�J�n
	mouse->Acquire();

	// �ŐV�̃}�E�X��񂾂������̂�1�t���[���O�̃L�[�{�[�h���Ƃ��ĕۑ�
	prevMouseInput = mouseInput;

	// �ŐV�̃}�E�X�����擾����
	mouse->GetDeviceState(sizeof(mouseInput), &mouseInput);

	POINT tempMousePos;
	GetCursorPos(&tempMousePos);

	// �E�B���h�E���W�ɕϊ�����
	ScreenToClient(renderWindow->GetHwnd(), &tempMousePos);
	mousePos.x = static_cast<float>(tempMousePos.x);
	mousePos.y = static_cast<float>(tempMousePos.y);
}
