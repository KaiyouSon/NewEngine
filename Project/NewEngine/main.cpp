#include "NewEngine.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	SetWindowTitle("PONDERING");
	SetWindowSize({ 1920, 1080 });
	SetBackGroundColor(25.5, 63.75, 127.5);
	SetFrameRate(60);
	NewEngineInit();	// エンジンの初期化

	// ゲームループ
	while (true)
	{
		NewEngineUpda();		// エンジンの更新処理
		NewEnginePreDraw();		// エンジン描画前処理
		NewEneineDraw();		// エンジンの描画処理
		NewEnginePostDraw();	// エンジン描画後処理

		// XボタンもしくはESCキーでゲームループを抜ける
		if (ProcessMessage() || Key::GetKey(DIK_ESCAPE))
		{
			break;
		}

		FrameRateUpdate();
	}

	NewEngineEnd();	// エンジンのエンド処理

	// 正常終了
	return 0;
}