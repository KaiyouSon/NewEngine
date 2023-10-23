#pragma once
#include "Vec2.h"
#include "Util.h"
#include <Windows.h>
#include <string>

// ゲームウィンドウを管理するクラス
class RenderWindow : public Singleton<RenderWindow>
{
private:
    Vec2 mSize;        // ウィンドウのサイズ
    std::string mTitle;  // タイトル文字列

    WNDCLASSEX mWndClass;  // ウィンドウクラス
    HWND mHwnd;           // ウィンドウハンドル
    MSG mMsg;             // メッセージ構造体

public:
    RenderWindow();

    // ゲームウィンドウの作成
    void CreateGameWindow();

    // ゲームウィンドウの終了
    void TerminateGameWindow();

    // ウィンドウメッセージの処理
    uint32_t ProcessMessage();

public: // プロパティ設定
    void SetWindowTitle(const std::string& title);
    void SetWindowSize(const Vec2 size);

public: // プロパティ取得
    Vec2 GetWindowSize();
    HWND GetHwnd();
    HINSTANCE GetHInstance();

private:
    friend Singleton<RenderWindow>;
};

// ウィンドウプロシージャ関数の宣言
LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);