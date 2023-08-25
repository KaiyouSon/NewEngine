#include "NewEngine.h"
#include "LogoutMenu.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	NewEngineSetting setting;
	setting.windowTitle = "PON_DE_RING";
	setting.windowSize = Vec2(1920, 1080);
	setting.bgColor = Color(25.5, 63.75, 127.5);
	setting.frameRate = 60.f;

	std::unique_ptr<NewEngine> newEngine = std::make_unique<NewEngine>(setting);
	newEngine->Init();	// エンジンの初期化

	// ゲームループ
	while (true)
	{
		newEngine->Update();	// エンジンの更新処理
		newEngine->PrevDraw();	// エンジン描画前処理
		newEngine->Draw();		// エンジンの描画処理
		newEngine->PostDraw();	// エンジン描画後処理

		bool isCloseGame =
			LogoutMenu::GetisEnd() == true &&
			LogoutMenu::GetSelect() == LogoutMenu::Select::CloseGame;

		if (isCloseGame == true)
		{
			break;
		}

		if (newEngine->ProcessMessage())
		{
			break;
		}

#ifdef _DEBUG
		// ESCキーでゲームループを抜ける
		if (Key::GetKey(DIK_ESCAPE))
		{
			break;
		}
#endif // DEBUG

		newEngine->FrameControl();
	}

	// 正常終了
	return 0;
}