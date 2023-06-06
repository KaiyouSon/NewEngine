#include "InputManager.h"
#include "MouseInput.h"
#include "RenderWindow.h"
#include <cassert>

#pragma region ���̑��̏���

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

#pragma endregion

#pragma region �}�E�X�̏���

// �N���b�N���Ă����
bool MouseInput::GetClick(const MouseCodo mouseCodo)
{
	bool isClick = (GetInstance()->mouseInput.rgbButtons[(int)mouseCodo] & 0x80) != 0;

	return isClick;
}

// �N���b�N�����u��
bool MouseInput::GetClickDown(const MouseCodo mouseCodo)
{
	bool isClick = (GetInstance()->mouseInput.rgbButtons[(int)mouseCodo] & 0x80) != 0;
	bool isPrevClick = (GetInstance()->prevMouseInput.rgbButtons[(int)mouseCodo] & 0x80) != 0;

	return isClick && !isPrevClick;
}

// �N���b�N���I������u��
bool MouseInput::GetClickUp(const MouseCodo mouseCodo)
{
	bool isClick = (GetInstance()->mouseInput.rgbButtons[(int)mouseCodo] & 0x80) != 0;
	bool isPrevClick = (GetInstance()->prevMouseInput.rgbButtons[(int)mouseCodo] & 0x80) != 0;

	return !isClick && isPrevClick;
}

// �}�E�X�̍��W
Vec2 MouseInput::GetPos()
{
	return GetInstance()->mousePos;
}

// �}�E�X�̓����Ă���x�N�g��
Vec2 MouseInput::GetMoveVec()
{
	return { (float)GetInstance()->mouseInput.lX, (float)GetInstance()->mouseInput.lY };
}

// �}�E�X�z�C���̓����Ă���x�N�g��
float MouseInput::GetWheelMoveVec()
{
	return (float)GetInstance()->mouseInput.lZ;
}

#pragma endregion