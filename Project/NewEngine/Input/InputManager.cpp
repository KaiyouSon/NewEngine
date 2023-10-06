#include "InputManager.h"
#include "RenderWindow.h"
#include <cassert>
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

void InputManager::Init()
{
	HRESULT result;
	RenderWindow* renderWindow = RenderWindow::GetInstance().get();

	// DirectInput縺ｮ蛻晄悄蛹・
	result = DirectInput8Create(
		renderWindow->GetHInstance(),
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&mDirectInput, nullptr);
	assert(SUCCEEDED(result));

	JoypadInput::GetInstance()->Init();
	KeyBoardInput::GetInstance()->Init();
	MouseInput::GetInstance()->Init();
}

void InputManager::Update()
{
	JoypadInput::GetInstance()->Update();
	KeyBoardInput::GetInstance()->Update();
	MouseInput::GetInstance()->Update();
}

IDirectInput8* InputManager::GetDirectInput()
{
	return mDirectInput.Get();
}
