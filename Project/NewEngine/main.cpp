#include "NewEngine.h"
#include "LogoutMenu.h"

// Windows用メイン関数（ウィンドウを作成し、メインループを実行するための関数）
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	NewEngineSetting setting;
	setting.windowTitle = "PON_DE_RING";
	setting.windowSize = Vec2(1920, 1080);
	setting.bgColor = Color::black;
	setting.frameRate = 60.f;

	std::unique_ptr<NewEngine> newEngine = std::make_unique<NewEngine>(setting);
	newEngine->Init(); // エンジンの初期化

	while (true)
	{
		newEngine->Update();    // エンジンの更新処理
		newEngine->PrevDraw();  // エンジンの前描画処理
		newEngine->Draw();      // エンジンの描画処理
		newEngine->PostDraw();  // エンジンの後描画処理

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
		// デバッグモード時にESCキーが押されたら終了
		if (Key::GetKey(DIK_ESCAPE))
		{
			break;
		}
#endif

		newEngine->FrameControl();
	}

	newEngine->SetisClose(true);

	return 0;
}
