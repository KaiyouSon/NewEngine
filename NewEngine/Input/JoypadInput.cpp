#include "InputManager.h"
#include "JoypadInput.h"
#include "RenderWindow.h"
#include <cassert>
#include <dbt.h>
#pragma comment(lib, "xinput.lib")

bool JoypadInput::isInsertPad = false;
int JoypadInput::padIndex = 0;

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

JoypadInput::JoypadInput() /*: joypad(nullptr), padInput({}), prevPadInput({})*/
{
}

void JoypadInput::SetJoyStick()
{
	RenderWindow* renderWindow = RenderWindow::GetInstance().get();

	HRESULT result;
	// ジョイパットデバイスの列挙
	result = InputManager::GetInstance()->GetDirectInput()->
		EnumDevices(
			DI8DEVCLASS_GAMECTRL,
			DeviceFindCallBack,
			this,
			DIEDFL_ATTACHEDONLY);
	assert(SUCCEEDED(result));
}

void JoypadInput::Init()
{
	SetJoyStick();

	// 抜き差し検知
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

		// ジョイパット情報の取得開始
		joypadObjs[i].joypad->Acquire();

		// デバイスのポーリングを行う
		joypadObjs[i].joypad->Poll();

		// 最新のパット情報だったものは1フレーム前のキーボード情報として保存
		joypadObjs[i].prevPadInput = joypadObjs[i].padInput;

		// 最新のジョイパット情報を取得する
		joypadObjs[i].joypad->GetDeviceState(sizeof(joypadObjs[i].padInput), &joypadObjs[i].padInput);
	}
}

// デバイス発見時に実行される
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

	// 入力データ形式のセット
	result = joypad->SetDataFormat(&c_dfDIJoystick2);
	assert(SUCCEEDED(result));

	// 排他制御レベルのセット
	result = joypad->SetCooperativeLevel(
		RenderWindow::GetInstance()->GetHwnd(),
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	assert(SUCCEEDED(result));

	// 軸のモードを絶対軸に設定
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(DIPROPDWORD);
	diprop.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.diph.dwObj = 0;
	diprop.dwData = DIPROPAXISMODE_ABS;
	joypad->SetProperty(DIPROP_AXISMODE, &diprop.diph);

	//軸の当たりの範囲を設定
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

