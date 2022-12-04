#include "NewEngine.h"
#include "main2.h"
#include "Util.h"

// Windows�A�v���ł̃G���g���[�|�C���g(main�֐�)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	SetWindowTitle("NewEngine");
	SetWindowSize({ 1280, 720 });
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

		// X�{�^����������ESC�L�[�ŃQ�[�����[�v�𔲂���
		if (ProcessMessage() || Key::GetKey(DIK_ESCAPE))
		{
			break;
		}

		FrameRateUpdate();
	}

	NewEngineEnd();	// �G���W���̃G���h����

	// ����I��
	return 0;
}