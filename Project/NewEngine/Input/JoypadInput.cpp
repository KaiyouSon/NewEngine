#include "InputManager.h"
#include "JoypadInput.h"
#include "RenderWindow.h"
#include <cassert>
#include <dbt.h>

#pragma region �ÓI�����o�ϐ�

bool JoypadInput::sIsInsertPad_ = false;
uint32_t JoypadInput::sPadIndex_ = 0;

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
	minButton_((int)PadCode::ButtonA), maxButton_((int)PadCode::ButtonR1)
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
	if (sIsInsertPad_ == true)
	{
		joypadObjs_.clear();
		SetJoyStick();
		sIsInsertPad_ = false;
	}

	for (int i = 0; i < joypadObjs_.size(); i++)
	{
		if (joypadObjs_[i].joypad == nullptr) continue;

		// �W���C�p�b�g���̎擾�J�n
		joypadObjs_[i].joypad->Acquire();

		// �f�o�C�X�̃|�[�����O���s��
		joypadObjs_[i].joypad->Poll();

		// �ŐV�̃p�b�g��񂾂������̂�1�t���[���O�̃L�[�{�[�h���Ƃ��ĕۑ�
		joypadObjs_[i].prevPadInput = joypadObjs_[i].padInput;

		// �ŐV�̃W���C�p�b�g�����擾����
		joypadObjs_[i].joypad->GetDeviceState(sizeof(joypadObjs_[i].padInput), &joypadObjs_[i].padInput);
	}
}

// �R���g���[���[�ڑ��������̃R�[���o�b�N
BOOL CALLBACK JoypadInput::DeviceFindCallBack(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext)
{
	HRESULT result;
	JoypadInput* joypadInput = static_cast<JoypadInput*>(pContext);
	Microsoft::WRL::ComPtr<IDirectInputDevice8>  joypad;
	result = InputManager::GetInstance()->GetDirectInput()->
		CreateDevice(pdidInstance->guidInstance, &joypad, nullptr);

	if (FAILED(result)) return DIENUM_CONTINUE;

	DIDEVICEINSTANCE instance;
	joypad->GetDeviceInfo(&instance);
	joypadInput->joypadObjs_.emplace_back(joypad);

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
	diprg.diph.dwObj = DIJOFS_RZ;
	joypad->SetProperty(DIPROP_RANGE, &diprg.diph);

	sPadIndex_++;

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
bool JoypadInput::GetButton(const PadCode padCode, const int sPadIndex_)
{
	// �ڑ����Ă��邩
	if (GetisLinkPad() == false) return false;

	if (padCode == PadCode::ButtonLeft)
	{
		return GetInstance()->joypadObjs_[sPadIndex_].padInput.rgdwPOV[0] == 27000;
	}
	else if (padCode == PadCode::ButtonRight)
	{
		return GetInstance()->joypadObjs_[sPadIndex_].padInput.rgdwPOV[0] == 9000;
	}
	else if (padCode == PadCode::ButtonUp)
	{
		return GetInstance()->joypadObjs_[sPadIndex_].padInput.rgdwPOV[0] == 0;
	}
	else if (padCode == PadCode::ButtonDown)
	{
		return GetInstance()->joypadObjs_[sPadIndex_].padInput.rgdwPOV[0] == 18000;
	}

	// �Y�����̃N�����v����
	int min = GetInstance()->minButton_;
	int max = GetInstance()->maxButton_;
	PadCode codo = (PadCode)Clamp((float)padCode, (float)min, (float)max);

	return GetInstance()->joypadObjs_[sPadIndex_].padInput.rgbButtons[(int)codo] & 0x80;
}

// �{�^�����������u��
bool JoypadInput::GetButtonDown(const PadCode padCode, const int sPadIndex_)
{
	// �ڑ����Ă��邩
	if (GetisLinkPad() == false) return false;

	if (padCode == PadCode::ButtonLeft)
	{
		return (GetInstance()->joypadObjs_[sPadIndex_].padInput.rgdwPOV[0] == 27000) &&
			!(GetInstance()->joypadObjs_[sPadIndex_].prevPadInput.rgdwPOV[0] == 27000);
	}
	else if (padCode == PadCode::ButtonRight)
	{
		return GetInstance()->joypadObjs_[sPadIndex_].padInput.rgdwPOV[0] == 9000 &&
			!(GetInstance()->joypadObjs_[sPadIndex_].prevPadInput.rgdwPOV[0] == 9000);
	}
	else if (padCode == PadCode::ButtonUp)
	{
		return GetInstance()->joypadObjs_[sPadIndex_].padInput.rgdwPOV[0] == 0 &&
			!(GetInstance()->joypadObjs_[sPadIndex_].prevPadInput.rgdwPOV[0] == 0);
	}
	else if (padCode == PadCode::ButtonDown)
	{
		return GetInstance()->joypadObjs_[sPadIndex_].padInput.rgdwPOV[0] == 18000 &&
			!(GetInstance()->joypadObjs_[sPadIndex_].prevPadInput.rgdwPOV[0] == 18000);
	}

	if (padCode == PadCode::RightStick)
	{
		return (GetInstance()->joypadObjs_[sPadIndex_].padInput.rgbButtons[9] & 0x80) &&
			!(GetInstance()->joypadObjs_[sPadIndex_].prevPadInput.rgbButtons[9] & 0x80);
	}

	// �Y�����̃N�����v����
	int min = GetInstance()->minButton_;
	int max = GetInstance()->maxButton_;
	PadCode codo = (PadCode)Clamp((float)padCode, (float)min, (float)max);

	return (GetInstance()->joypadObjs_[sPadIndex_].padInput.rgbButtons[(int)codo] & 0x80) &&
		!(GetInstance()->joypadObjs_[sPadIndex_].prevPadInput.rgbButtons[(int)codo] & 0x80);
}

// �{�^���𗣂����u��
bool JoypadInput::GetButtonUp(const PadCode padCode, const int sPadIndex_)
{
	// �ڑ����Ă��邩
	if (GetisLinkPad() == false) return false;

	// �Y�����̃N�����v����
	int min = GetInstance()->minButton_;
	int max = GetInstance()->maxButton_;
	PadCode codo = (PadCode)Clamp((float)padCode, (float)min, (float)max);

	return !(GetInstance()->joypadObjs_[sPadIndex_].padInput.rgbButtons[(int)codo] & 0x80) &&
		(GetInstance()->joypadObjs_[sPadIndex_].prevPadInput.rgbButtons[(int)codo] & 0x80);
}

// �����̃{�^�����������u��
bool JoypadInput::GetAnyButtonDown(const int padIndex)
{
	// �ڑ����Ă��邩
	if (GetisLinkPad() == false) return false;

	// �{�^���̏�Ԃ��X�V
	for (uint32_t i = 0; i < 16; ++i)
	{
		bool down = (GetInstance()->joypadObjs_[padIndex].padInput.rgbButtons[i] & 0x80);
		bool up = !(GetInstance()->joypadObjs_[padIndex].prevPadInput.rgbButtons[i] & 0x80);

		if (down == true && up == true)
		{
			return true;
		}
	}

	for (uint32_t i = 1; i < 5; i++)
	{
		bool down = (GetInstance()->joypadObjs_[padIndex].padInput.rgdwPOV[0] == i * 9000);
		bool up = !(GetInstance()->joypadObjs_[padIndex].prevPadInput.rgdwPOV[0] == i * 9000);

		if (down == true && up == true)
		{
			return true;
		}
	}

	return false;
}

#pragma endregion

#pragma region �X�e�B�b�N�֘A

// �X�e�B�b�N��|���Ă����
Vec2 JoypadInput::GetStick(const PadCode padCode, const float length, const int sPadIndex_)
{
	// �ڑ����Ă��邩
	if (GetisLinkPad() == false) return 0;

	Vec2 stick = 0;
	if (padCode == PadCode::LeftStick)
	{
		stick.x = (float)GetInstance()->joypadObjs_[sPadIndex_].padInput.lX;
		stick.y = (float)GetInstance()->joypadObjs_[sPadIndex_].padInput.lY;
	}
	else if (padCode == PadCode::RightStick)
	{
		stick.x = (float)GetInstance()->joypadObjs_[sPadIndex_].padInput.lRx;
		stick.y = (float)GetInstance()->joypadObjs_[sPadIndex_].padInput.lRy;
	}

	// �w�肵�������𒴂����ꍇ�ɂ����Ԃ��Ȃ�
	if (stick.Length() > fabsf(length))
	{
		return stick;
	}

	return 0;
}

// �X�e�B�b�N��|�����u��
Vec2 JoypadInput::GetStickDown(const PadCode padCode, const float length, const int sPadIndex_)
{
	// �ڑ����Ă��邩
	if (GetisLinkPad() == false) return 0;

	Vec2 stick = 0;
	Vec2 prevStick = 0;
	if (padCode == PadCode::LeftStick)
	{
		stick.x = (float)GetInstance()->joypadObjs_[sPadIndex_].padInput.lX;
		stick.y = (float)GetInstance()->joypadObjs_[sPadIndex_].padInput.lY;

		prevStick.x = (float)GetInstance()->joypadObjs_[sPadIndex_].prevPadInput.lX;
		prevStick.y = (float)GetInstance()->joypadObjs_[sPadIndex_].prevPadInput.lY;
	}
	else if (padCode == PadCode::RightStick)
	{
		stick.x = (float)GetInstance()->joypadObjs_[sPadIndex_].padInput.lRx;
		stick.y = (float)GetInstance()->joypadObjs_[sPadIndex_].padInput.lRy;

		prevStick.x = (float)GetInstance()->joypadObjs_[sPadIndex_].prevPadInput.lRx;
		prevStick.y = (float)GetInstance()->joypadObjs_[sPadIndex_].prevPadInput.lRy;
	}

	// �w�肵�������𒴂����ꍇ�ɂ����Ԃ��Ȃ�
	if (!(stick.Length() > fabsf(length)) && (prevStick.Length() > fabsf(length)))
	{
		return stick;
	}

	return 0;
}

// �X�e�B�b�N�𗣂����u��
Vec2 JoypadInput::GetStickUp(const PadCode padCode, const float length, const int sPadIndex_)
{
	// �ڑ����Ă��邩
	if (GetisLinkPad() == false) return 0;

	Vec2 stick = 0;
	Vec2 prevStick = 0;
	if (padCode == PadCode::LeftStick)
	{
		stick.x = (float)GetInstance()->joypadObjs_[sPadIndex_].padInput.lX;
		stick.y = (float)GetInstance()->joypadObjs_[sPadIndex_].padInput.lY;

		prevStick.x = (float)GetInstance()->joypadObjs_[sPadIndex_].prevPadInput.lX;
		prevStick.y = (float)GetInstance()->joypadObjs_[sPadIndex_].prevPadInput.lY;
	}
	else if (padCode == PadCode::RightStick)
	{
		stick.x = (float)GetInstance()->joypadObjs_[sPadIndex_].padInput.lRx;
		stick.y = (float)GetInstance()->joypadObjs_[sPadIndex_].padInput.lRy;

		prevStick.x = (float)GetInstance()->joypadObjs_[sPadIndex_].prevPadInput.lRx;
		prevStick.y = (float)GetInstance()->joypadObjs_[sPadIndex_].prevPadInput.lRy;
	}

	// �w�肵�������𒴂����ꍇ�ɂ����Ԃ��Ȃ�
	if (!(stick.Length() > fabsf(length)) && (prevStick.Length() > fabsf(length)))
	{
		return stick;
	}

	return 0;
}

Vec3 JoypadInput::GetStickVec3(const PadCode padCode, const float length, const int padIndex)
{
	// �ڑ����Ă��邩
	if (GetisLinkPad() == false) return 0;

	Vec3 stick = 0;
	if (padCode == PadCode::LeftStick)
	{
		stick.x = (float)GetInstance()->joypadObjs_[padIndex].padInput.lX;
		stick.z = (float)GetInstance()->joypadObjs_[padIndex].padInput.lY;
	}
	else if (padCode == PadCode::RightStick)
	{
		stick.x = (float)GetInstance()->joypadObjs_[padIndex].padInput.lRx;
		stick.z = (float)GetInstance()->joypadObjs_[padIndex].padInput.lRy;
	}

	// �w�肵�������𒴂����ꍇ�ɂ����Ԃ��Ȃ�
	if (stick.Length() > fabsf(length))
	{
		return stick;
	}

	return 0;
}

#pragma endregion

#pragma region �g���K�[�֘A

// �g���K�[�����������
float JoypadInput::GetTrigger(const PadCode padCode, const float length, const int sPadIndex_)
{
	// �ڑ����Ă��邩
	if (GetisLinkPad() == false) return 0;

	float trigger = 0;
	if (padCode == PadCode::LeftTrigger)
	{
		trigger = (float)GetInstance()->joypadObjs_[sPadIndex_].padInput.lZ;
		if (trigger > length)
		{
			return trigger;
		}
	}
	else if (padCode == PadCode::RightTrigger)
	{
		trigger = (float)GetInstance()->joypadObjs_[sPadIndex_].padInput.lZ;
		if (trigger < -length)
		{
			return trigger;
		}
	}

	return 0;
}

// �g���K�[���������u��
bool JoypadInput::GetTriggerDown(const PadCode padCode, const float length, const int sPadIndex_)
{
	if (padCode == PadCode::LeftTrigger)
	{
		bool isGreater = (float)GetInstance()->joypadObjs_[sPadIndex_].padInput.lZ > length;
		bool isPrevGreater = (float)GetInstance()->joypadObjs_[sPadIndex_].prevPadInput.lZ > length;

		if (isGreater && !isPrevGreater)
		{
			return true;
		}
	}
	else if (padCode == PadCode::RightTrigger)
	{
		bool isLess = (float)GetInstance()->joypadObjs_[sPadIndex_].padInput.lZ < -length;
		bool isPrevLess = (float)GetInstance()->joypadObjs_[sPadIndex_].padInput.lZ < -length;

		if (isLess && !isPrevLess)
		{
			return true;
		}
	}

	return false;
}

// �g���K�[�𗣂����u��
bool JoypadInput::GetTriggerUp(const PadCode padCode, const float length, const int sPadIndex_)
{
	if (padCode == PadCode::LeftTrigger)
	{
		bool isGreater = (float)GetInstance()->joypadObjs_[sPadIndex_].padInput.lZ > length;
		bool isPrevGreater = (float)GetInstance()->joypadObjs_[sPadIndex_].prevPadInput.lZ > length;

		if (!isGreater && isPrevGreater)
		{
			return true;
		}
	}
	else if (padCode == PadCode::RightTrigger)
	{
		bool isLess = (float)GetInstance()->joypadObjs_[sPadIndex_].padInput.lZ < -length;
		bool isPrevLess = (float)GetInstance()->joypadObjs_[sPadIndex_].padInput.lZ < -length;

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
bool JoypadInput::GetisLinkPad(const int sPadIndex_)
{
	// ��
	return GetInstance()->joypadObjs_.size() > 0;
}

#pragma endregion