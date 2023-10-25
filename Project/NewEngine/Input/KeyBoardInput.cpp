#include "InputManager.h"
#include "KeyBoardInput.h"
#include "RenderWindow.h"
#include <cassert>
#include <memory>

void KeyBoardInput::Init()
{
    HRESULT result;
    RenderWindow* renderWindow = RenderWindow::GetInstance().get();

    // キーボードデバイスの作成と初期化
    result = InputManager::GetInstance()->GetDirectInput()->
        CreateDevice(GUID_SysKeyboard, &mKeyboard, nullptr);
    assert(SUCCEEDED(result));

    // キーボードデバイスのデータフォーマットを設定
    result = mKeyboard->SetDataFormat(&c_dfDIKeyboard);  // デフォルトのデータフォーマット
    assert(SUCCEEDED(result));

    // キーボードデバイスの協調レベルを設定
    result = mKeyboard->SetCooperativeLevel(
        renderWindow->GetHwnd(),
        DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
    assert(SUCCEEDED(result));
}

void KeyBoardInput::Update()
{
    // キーボードデバイスの状態を取得
    mKeyboard->Acquire();

    // 前回のキーボードの状態を保存
    for (uint32_t i = 0; i < 256; ++i)
    {
        mPrevKeys[i] = mKeys[i];
    }

    // キーボードの状態を取得
    mKeyboard->GetDeviceState(sizeof(mKeys), mKeys.data());
}

// キーが押されているかを取得
bool KeyBoardInput::GetKey(const uint32_t key)
{
    // 指定されたキーの状態を返す
    uint32_t index = Clamp<uint32_t>(key, 0, 255);
    return (bool)GetInstance()->mKeys[index];
}

// キーが押された瞬間の状態を取得
bool KeyBoardInput::GetKeyDown(const uint32_t key)
{
    // 指定されたキーの押下状態を返す
    uint32_t index = Clamp<uint32_t>(key, 0, 255);
    return (bool)(GetInstance()->mKeys[index] && !GetInstance()->mPrevKeys[index]);
}

// キーが離された瞬間の状態を取得
bool KeyBoardInput::GetKeyUp(const uint32_t key)
{
    // 指定されたキーの離し状態を返す
    uint32_t index = Clamp<uint32_t>(key, 0, 255);
    return (bool)(!GetInstance()->mKeys[index] && GetInstance()->mPrevKeys[index]);
}