#include "InputManager.h"
#include "JoypadInput.h"
#include "RenderWindow.h"
#include <cassert>
#pragma comment(lib, "xinput.lib")

int JoypadInput::padIndex = 0;

JoypadInput::JoypadInput() /*: joypad(nullptr), padInput({}), prevPadInput({})*/
{
}

void JoypadInput::Init()
{
	RenderWindow* renderWindow = RenderWindow::GetInstance().get();

	HRESULT result;
	// �W���C�p�b�g�f�o�C�X�̗�
	result = InputManager::GetInstance()->GetDirectInput()->
		EnumDevices(
			DI8DEVCLASS_GAMECTRL,
			DeviceFindCallBack,
			this,
			DIEDFL_ATTACHEDONLY);
	assert(SUCCEEDED(result));
}

void JoypadInput::Update()
{
	for (int i = 0; i < jyopadObjs.size(); i++)
	{
		if (jyopadObjs[i].joypad == nullptr) continue;

		// �W���C�p�b�g���̎擾�J�n
		jyopadObjs[i].joypad->Acquire();

		// �ŐV�̃p�b�g��񂾂������̂�1�t���[���O�̃L�[�{�[�h���Ƃ��ĕۑ�
		jyopadObjs[i].prevPadInput = jyopadObjs[i].padInput;

		// �ŐV�̃W���C�p�b�g�����擾����
		jyopadObjs[i].joypad->GetDeviceState(sizeof(jyopadObjs[i].padInput), &jyopadObjs[i].padInput);
	}
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
	joypadInput->jyopadObjs[padIndex].joypad = joypad;

	//if (joypad == nullptr) return;

	// ���̓f�[�^�`���̃Z�b�g
	result = joypad->SetDataFormat(&c_dfDIJoystick2);
	assert(SUCCEEDED(result));

	// �r�����䃌�x���̃Z�b�g
	result = joypad->SetCooperativeLevel(
		RenderWindow::GetInstance()->GetHwnd(),
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
	diprg.lMin = -1000;
	diprg.lMax = 1000;
	diprg.diph.dwObj = DIJOFS_X;
	joypad->SetProperty(DIPROP_RANGE, &diprg.diph);
	diprg.diph.dwObj = DIJOFS_Y;
	joypad->SetProperty(DIPROP_RANGE, &diprg.diph);
	diprg.diph.dwObj = DIJOFS_Z;
	joypad->SetProperty(DIPROP_RANGE, &diprg.diph);
	diprg.diph.dwObj = DIJOFS_RX;
	joypad->SetProperty(DIPROP_RANGE, &diprg.diph);
	diprg.diph.dwObj = DIJOFS_RY;
	joypad->SetProperty(DIPROP_RANGE, &diprg.diph);

	padIndex++;

	return DIENUM_CONTINUE;
}
