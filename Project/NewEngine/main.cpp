#include "NewEngine.h"
#include "LogoutMenu.h"

// Windows�A�v���ł̃G���g���[�|�C���g(main�֐�)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	SetWindowTitle("PON_DE_RING");
	SetWindowSize({ 1920, 1080 });
	SetBackGroundColor(25.5, 63.75, 127.5);
	SetFrameRate(60);
	NewEngineInit();	// �G���W���̏�����

	// �Q�[�����[�v
	while (true)
	{
		NewEngineUpda();		// �G���W���̍X�V����
		NewEnginePreDraw();		// �G���W���`��O����
		NewEneineDraw();		// �G���W���̕`�揈��
		NewEnginePostDraw();	// �G���W���`��㏈��

		bool isCloseGame =
			LogoutMenu::GetisEnd() == true &&
			LogoutMenu::GetSelect() == LogoutMenu::Select::CloseGame;

		if (isCloseGame == true)
		{
			break;
		}

		if (ProcessMessage())
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

		FrameRateUpdate();
	}

	NewEngineEnd();	// �G���W���̃G���h����

	// ����I��
	return 0;
}