#include "NewEngine.h"
#include "LogoutMenu.h"

// Windows�A�v���ł̃G���g���[�|�C���g(main�֐�)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	NewEngineSetting setting;
	setting.windowTitle = "PON_DE_RING";
	setting.windowSize = Vec2(1920, 1080);
	setting.bgColor = Color(25.5, 63.75, 127.5);
	setting.frameRate = 60.f;

	std::unique_ptr<NewEngine> newEngine = std::make_unique<NewEngine>(setting);
	newEngine->Init();	// �G���W���̏�����

	// �Q�[�����[�v
	while (true)
	{
		newEngine->Update();	// �G���W���̍X�V����
		newEngine->PrevDraw();	// �G���W���`��O����
		newEngine->Draw();		// �G���W���̕`�揈��
		newEngine->PostDraw();	// �G���W���`��㏈��

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
		// ESC�L�[�ŃQ�[�����[�v�𔲂���
		if (Key::GetKey(DIK_ESCAPE))
		{
			break;
		}
#endif // DEBUG

		newEngine->FrameControl();
	}

	// ����I��
	return 0;
}