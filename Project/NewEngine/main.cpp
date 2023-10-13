#include "NewEngine.h"
#include "LogoutMenu.h"

// Windows繧｢繝励Μ縺ｧ縺ｮ繧ｨ繝ｳ繝医Μ繝ｼ繝昴う繝ｳ繝・main髢｢謨ｰ)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	NewEngineSetting setting;
	setting.windowTitle = "PON_DE_RING";
	setting.windowSize = Vec2(1920, 1080);
	setting.bgColor = Color::black;
	setting.frameRate = 60.f;

	std::unique_ptr<NewEngine> newEngine = std::make_unique<NewEngine>(setting);
	newEngine->Init();	// 繧ｨ繝ｳ繧ｸ繝ｳ縺ｮ蛻晄悄蛹・

	// 繧ｲ繝ｼ繝繝ｫ繝ｼ繝・
	while (true)
	{
		newEngine->Update();	// 繧ｨ繝ｳ繧ｸ繝ｳ縺ｮ譖ｴ譁ｰ蜃ｦ逅・
		newEngine->PrevDraw();	// 繧ｨ繝ｳ繧ｸ繝ｳ謠冗判蜑榊・逅・
		newEngine->Draw();		// 繧ｨ繝ｳ繧ｸ繝ｳ縺ｮ謠冗判蜃ｦ逅・
		newEngine->PostDraw();	// 繧ｨ繝ｳ繧ｸ繝ｳ謠冗判蠕悟・逅・

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
		// ESC繧ｭ繝ｼ縺ｧ繧ｲ繝ｼ繝繝ｫ繝ｼ繝励ｒ謚懊￠繧・
		if (Key::GetKey(DIK_ESCAPE))
		{
			break;
		}
#endif // DEBUG

		newEngine->FrameControl();
	}

	// 豁｣蟶ｸ邨ゆｺ・
	return 0;
}
