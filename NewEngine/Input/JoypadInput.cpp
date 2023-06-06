#include "InputManager.h"
#include "JoypadInput.h"
#include "RenderWindow.h"
#include <cassert>
#include <dbt.h>
#pragma comment(lib, "xinput.lib")

#pragma region �ÓI�����o�ϐ�

bool JoypadInput::isInsertPad = false;
int JoypadInput::padIndex = 0;

#pragma endregion

#pragma region �E�B���h�E�v���V�[�W��

LRESULT SubWindowProc(int code, WPARAM wParam, LPARAM lParam)
{
	if (code < 0)
	{
		return CallNextHookEx(NULL, code, wParam, lParam);
	}

	PCWPSTRUCT msg = PCWPSTRUCT(lParam);
	if (msg->message == WM_DEVICECHANGE)
	{
		switch (msg->wParam)
		{
		case DBT_DEVICEARRIVAL:
		case DBT_DEVICEREMOVECOMPLETE:
			Pad::GetInstance()->SetisInsertPad(true);
			break;
		}
	}

	return CallNextHookEx(NULL, code, wParam, lParam);
}

#pragma endregion

#pragma region ���̑��̏���

JoypadInput::JoypadInput() :
	minButton((int)PadCode::ButtonA), maxButton((int)PadCode::ButtonR1)
{
}
void JoypadInput::Init()
{
	SetJoyStick();

	// �����������m
	DEV_BROADCAST_DEVICEINTERFACE notificationFilter{};
	notificationFilter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
	notificationFilter.dbcc_size = sizeof(notificationFilter);

	HDEVNOTIFY notifyResult = RegisterDeviceNotification(
		RenderWindow::GetInstance()->GetHwnd(), &notificationFilter,
		DEVICE_NOTIFY_WINDOW_HANDLE | DEVICE_NOTIFY_ALL_INTERFACE_CLASSES);
	assert(!!notifyResult);

	SetWindowsHookExW(
		WH_CALLWNDPROC, (HOOKPROC)&SubWindowProc, GetModuleHandleW(NULL), GetCurrentThreadId());
}
void JoypadInput::Update()
{
	if (isInsertPad == true)
	{
		joypadObjs.clear();
		SetJoyStick();
		isInsertPad = false;
	}

	for (int i = 0; i < joypadObjs.size(); i++)
	{
		if (joypadObjs[i].joypad == nullptr) continue;

		// �W���C�p�b�g���̎擾�J�n
		joypadObjs[i].joypad->Acquire();

		// �f�o�C�X�̃|�[�����O���s��
		joypadObjs[i].joypad->Poll();

		// �ŐV�̃p�b�g��񂾂������̂�1�t���[���O�̃L�[�{�[�h���Ƃ��ĕۑ�
		joypadObjs[i].prevPadInput = joypadObjs[i].padInput;

		// �ŐV�̃W���C�p�b�g�����擾����
		joypadObjs[i].joypad->GetDeviceState(sizeof(joypadObjs[i].padInput), &joypadObjs[i].padInput);
	}
}

// �R���g���[���[�ڑ��������̃R�[���o�b�N
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
	joypadInput->joypadObjs.emplace_back(joypad);

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

// �R���g���[���[��ڑ����鏈��
void JoypadInput::SetJoyStick()
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

#pragma endregion

#pragma region �{�^���֘A

// �{�^���������Ă��
bool JoypadInput::GetButton(const PadCode padCode, const int padIndex)
{
	// �ڑ����Ă��邩
	if (GetisLinkPad() == false) return false;

	// �Y�����̃N�����v����
	int min = GetInstance()->minButton;
	int max = GetInstance()->maxButton;
	PadCode codo = (PadCode)Clamp((float)padCode, (float)min, (float)max);

	return GetInstance()->joypadObjs[padIndex].padInput.rgbButtons[(int)codo] & 0x80;

	//if (padCode == PadCodo::ButtonLeft)
	//{
	//	return GetInstance()->joypadObjs[padIndex].padInput.rgdwPOV[0] == 27000;
	//}
	//else if (padCode == PadCodo::ButtonRight)
	//{
	//	return GetInstance()->joypadObjs[padIndex].padInput.rgdwPOV[0] == 9000;
	//}
	//else if (padCode == PadCodo::ButtonUp)
	//{
	//	return GetInstance()->joypadObjs[padIndex].padInput.rgdwPOV[0] == 0;
	//}
	//else if (padCode == PadCodo::ButtonDown)
	//{
	//	return GetInstance()->joypadObjs[padIndex].padInput.rgdwPOV[0] == 18000;
	//}
	//else
	//{
	//}
}

// �{�^�����������u��
bool JoypadInput::GetButtonDown(const PadCode padCode, const int padIndex)
{
	// �ڑ����Ă��邩
	if (GetisLinkPad() == false) return false;

	// �Y�����̃N�����v����
	int min = GetInstance()->minButton;
	int max = GetInstance()->maxButton;
	PadCode codo = (PadCode)Clamp((float)padCode, (float)min, (float)max);

	return (GetInstance()->joypadObjs[padIndex].padInput.rgbButtons[(int)codo] & 0x80) &&
		!(GetInstance()->joypadObjs[padIndex].prevPadInput.rgbButtons[(int)codo] & 0x80);
}

// �{�^���𗣂����u��
bool JoypadInput::GetButtonUp(const PadCode padCode, const int padIndex)
{
	// �ڑ����Ă��邩
	if (GetisLinkPad() == false) return false;

	// �Y�����̃N�����v����
	int min = GetInstance()->minButton;
	int max = GetInstance()->maxButton;
	PadCode codo = (PadCode)Clamp((float)padCode, (float)min, (float)max);

	return !(GetInstance()->joypadObjs[padIndex].padInput.rgbButtons[(int)codo] & 0x80) &&
		(GetInstance()->joypadObjs[padIndex].prevPadInput.rgbButtons[(int)codo] & 0x80);
}

#pragma endregion

#pragma region �X�e�B�b�N�֘A

// �X�e�B�b�N��|���Ă����
Vec2 JoypadInput::GetStick(const PadCode padCode, const int padIndex)
{
	// �ڑ����Ă��邩
	if (GetisLinkPad() == false) return 0;

	Vec2 stick = 0;
	if (padCode == PadCode::LeftStick)
	{
		stick.x = (float)GetInstance()->joypadObjs[padIndex].padInput.lX;
		stick.y = (float)GetInstance()->joypadObjs[padIndex].padInput.lY;
	}
	else if (padCode == PadCode::RightStick)
	{
		stick.x = (float)GetInstance()->joypadObjs[padIndex].padInput.lRx;
		stick.y = (float)GetInstance()->joypadObjs[padIndex].padInput.lRy;
	}

	return stick;
}

// �X�e�B�b�N��|�����u��
Vec2 JoypadInput::GetStickDown(const PadCode padCode, const float length, const int padIndex)
{
	// �ڑ����Ă��邩
	if (GetisLinkPad() == false) return 0;

	Vec2 stick = 0;
	Vec2 prevStick = 0;
	if (padCode == PadCode::LeftStick)
	{
		stick.x = (float)GetInstance()->joypadObjs[padIndex].padInput.lX;
		stick.y = (float)GetInstance()->joypadObjs[padIndex].padInput.lY;

		prevStick.x = (float)GetInstance()->joypadObjs[padIndex].prevPadInput.lX;
		prevStick.y = (float)GetInstance()->joypadObjs[padIndex].prevPadInput.lY;
	}
	else if (padCode == PadCode::RightStick)
	{
		stick.x = (float)GetInstance()->joypadObjs[padIndex].padInput.lRx;
		stick.y = (float)GetInstance()->joypadObjs[padIndex].padInput.lRy;

		prevStick.x = (float)GetInstance()->joypadObjs[padIndex].prevPadInput.lRx;
		prevStick.y = (float)GetInstance()->joypadObjs[padIndex].prevPadInput.lRy;
	}

	// �w�肵�������𒴂����ꍇ�ɂ����Ԃ��Ȃ�
	if (!(stick.Length() > fabsf(length)) && (prevStick.Length() > fabsf(length)))
	{
		return stick;
	}

	return 0;
}

// �X�e�B�b�N�𗣂����u��
Vec2 JoypadInput::GetStickUp(const PadCode padCode, const float length, const int padIndex)
{
	// �ڑ����Ă��邩
	if (GetisLinkPad() == false) return 0;

	Vec2 stick = 0;
	Vec2 prevStick = 0;
	if (padCode == PadCode::LeftStick)
	{
		stick.x = (float)GetInstance()->joypadObjs[padIndex].padInput.lX;
		stick.y = (float)GetInstance()->joypadObjs[padIndex].padInput.lY;

		prevStick.x = (float)GetInstance()->joypadObjs[padIndex].prevPadInput.lX;
		prevStick.y = (float)GetInstance()->joypadObjs[padIndex].prevPadInput.lY;
	}
	else if (padCode == PadCode::RightStick)
	{
		stick.x = (float)GetInstance()->joypadObjs[padIndex].padInput.lRx;
		stick.y = (float)GetInstance()->joypadObjs[padIndex].padInput.lRy;

		prevStick.x = (float)GetInstance()->joypadObjs[padIndex].prevPadInput.lRx;
		prevStick.y = (float)GetInstance()->joypadObjs[padIndex].prevPadInput.lRy;
	}

	// �w�肵�������𒴂����ꍇ�ɂ����Ԃ��Ȃ�
	if (!(stick.Length() > fabsf(length)) && (prevStick.Length() > fabsf(length)))
	{
		return stick;
	}

	return 0;
}

#pragma endregion

#pragma region �g���K�[�֘A

// �g���K�[�����������
float JoypadInput::GetTrigger(const PadCode padCode, const int padIndex)
{
	if (padCode == PadCode::LeftTrigger)
	{
		return (float)GetInstance()->joypadObjs[padIndex].padInput.lZ;
	}
	else if (padCode == PadCode::RightTrigger)
	{
		return -(float)GetInstance()->joypadObjs[padIndex].padInput.lZ;
	}

	return 0;
}

// �g���K�[���������u��
bool JoypadInput::GetTriggerDown(const PadCode padCode, const float length, const int padIndex)
{
	if (padCode == PadCode::LeftTrigger)
	{
		bool isGreater = (float)GetInstance()->joypadObjs[padIndex].padInput.lZ > length;
		bool isPrevGreater = (float)GetInstance()->joypadObjs[padIndex].prevPadInput.lZ > length;

		if (isGreater && !isPrevGreater)
		{
			return true;
		}
	}
	else if (padCode == PadCode::RightTrigger)
	{
		bool isLess = (float)GetInstance()->joypadObjs[padIndex].padInput.lZ < -length;
		bool isPrevLess = (float)GetInstance()->joypadObjs[padIndex].padInput.lZ < -length;

		if (isLess && !isPrevLess)
		{
			return true;
		}
	}

	return false;
}

// �g���K�[�𗣂����u��
bool JoypadInput::GetTriggerUp(const PadCode padCode, const float length, const int padIndex)
{
	if (padCode == PadCode::LeftTrigger)
	{
		bool isGreater = (float)GetInstance()->joypadObjs[padIndex].padInput.lZ > length;
		bool isPrevGreater = (float)GetInstance()->joypadObjs[padIndex].prevPadInput.lZ > length;

		if (!isGreater && isPrevGreater)
		{
			return true;
		}
	}
	else if (padCode == PadCode::RightTrigger)
	{
		bool isLess = (float)GetInstance()->joypadObjs[padIndex].padInput.lZ < -length;
		bool isPrevLess = (float)GetInstance()->joypadObjs[padIndex].padInput.lZ < -length;

		if (!isLess && isPrevLess)
		{
			return true;
		}
	}

	return false;
}

#pragma endregion

#pragma region ���̑��擾�֘A

// �R���g���[���[�ڑ����Ă��邩
bool JoypadInput::GetisLinkPad(const int padIndex)
{
	// ��
	return GetInstance()->joypadObjs.size() > 0;
}

#pragma endregion