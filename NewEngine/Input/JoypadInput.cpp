#include "InputManager.h"
#include "JoypadInput.h"
#include "RenderWindow.h"
#include <cassert>
#pragma comment(lib, "xinput.lib")

void JoypadInput::Initialize()
{
	HRESULT result;
	RenderWindow* renderWindow = RenderWindow::GetInstance().get();

	// �W���C�p�b�g�f�o�C�X�̗�
	result = InputManager::GetInstance()->GetDirectInput()->
		EnumDevices(
			DI8DEVCLASS_GAMECTRL,
			DeviceFindCallBack,
			this,
			DIEDFL_ATTACHEDONLY);
	assert(SUCCEEDED(result));

	if (joypad == nullptr) return;

	// ���̓f�[�^�`���̃Z�b�g
	result = joypad->SetDataFormat(&c_dfDIJoystick2);
	assert(SUCCEEDED(result));

	// �r�����䃌�x���̃Z�b�g
	result = joypad->SetCooperativeLevel(
		renderWindow->GetHwnd(),
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	assert(SUCCEEDED(result));

	// ���̃��[�h���Ύ��ɐݒ�
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(DIPROPDWORD);
	diprop.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.diph.dwObj = 0;
	diprop.dwData = DIPROPAXISMODE_ABS;
	joypad->SetProperty(DIPROP_AXISMODE, &diprop.diph);

	//���̓�����͈̔͂�ݒ�
	DIPROPRANGE diprg;
	diprg.diph.dwSize = sizeof(DIPROPRANGE);
	diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	diprg.diph.dwHow = DIPH_BYOFFSET;
	diprg.lMin = -32768;
	diprg.lMax = 32767;
	diprg.diph.dwObj = DIJOFS_X;
	joypad->SetProperty(DIPROP_RANGE, &diprg.diph);
	diprg.diph.dwObj = DIJOFS_Y;
	joypad->SetProperty(DIPROP_RANGE, &diprg.diph);
}

void JoypadInput::Update()
{
	if (joypad == nullptr) return;

	// �W���C�p�b�g���̎擾�J�n
	joypad->Acquire();

	// �ŐV�̃p�b�g��񂾂������̂�1�t���[���O�̃L�[�{�[�h���Ƃ��ĕۑ�
	oldPadInput = padInput;

	// �ŐV�̃W���C�p�b�g�����擾����
	joypad->GetDeviceState(sizeof(padInput), &padInput);
}

bool JoypadInput::GetButton(const int& buttonNumber)
{
	return padInput.rgbButtons[buttonNumber] & 0x80;
}

bool JoypadInput::GetButtonTrigger(const int& buttonNumber)
{
	return (padInput.rgbButtons[buttonNumber] & 0x80) &&
		!(oldPadInput.rgbButtons[buttonNumber] & 0x80);
}

bool JoypadInput::GetButtonReleased(const int& buttonNumber)
{
	return !(padInput.rgbButtons[buttonNumber] & 0x80) &&
		(oldPadInput.rgbButtons[buttonNumber] & 0x80);
}

Vec2 JoypadInput::GetLeftStickParam()
{
	return Vec2(padInput.lX, padInput.lY);
}

Vec2 JoypadInput::GetLeftStickVec()
{
	return Vec2(padInput.lX, padInput.lY).Normalized();
}

Vec2 JoypadInput::GetRightStickParam()
{
	return Vec2(padInput.lRx, padInput.lRy);
}

Vec2 JoypadInput::GetRightStickVec()
{
	return Vec2(padInput.lRx, padInput.lRy).Normalized();
}

// �f�o�C�X�������Ɏ��s�����
BOOL CALLBACK JoypadInput::DeviceFindCallBack(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext)
{
	HRESULT result;
	//Input* input = static_cast<Input*>(pContext);
	JoypadInput* joypadInput = static_cast<JoypadInput*>(pContext);
	Microsoft::WRL::ComPtr<IDirectInputDevice8>  joypad;
	result = InputManager::GetInstance()->GetDirectInput()->
		CreateDevice(pdidInstance->guidInstance, &joypad, nullptr);
	if (FAILED(result)) return DIENUM_CONTINUE;

	DIDEVICEINSTANCE instance;
	joypad->GetDeviceInfo(&instance);
	joypadInput->joypad = joypad;

	return DIENUM_CONTINUE;
}