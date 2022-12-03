#include "InputManager.h"
#include "RenderWindow.h"
#include <cassert>
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

void InputManager::Initialize()
{
	HRESULT result;
	RenderWindow* renderWindow = RenderWindow::GetInstance().get();

	// DirectInput‚Ì‰Šú‰»
	result = DirectInput8Create(
		renderWindow->GetHInstance(),
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&directInput, nullptr);
	assert(SUCCEEDED(result));

	JoypadInput::GetInstance()->Initialize();
	KeyBoardInput::GetInstance()->Initialize();
	MouseInput::GetInstance()->Initialize();
}

void InputManager::Update()
{
	JoypadInput::GetInstance()->Update();
	KeyBoardInput::GetInstance()->Update();
	MouseInput::GetInstance()->Update();
}

InputManager* inputManager = InputManager::GetInstance().get();
JoypadInput* pad = JoypadInput::GetInstance().get();
KeyBoardInput* key = KeyBoardInput::GetInstance().get();
MouseInput* mouse = MouseInput::GetInstance().get();
