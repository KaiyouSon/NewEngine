#include "NewEngine.h"
#include "main2.h"
#include "InputManager.h"
#include "Util.h"

// Windows�A�v���ł̃G���g���[�|�C���g(main�֐�)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	FrameRate* frameRate = FrameRate::GetInstance().get();

	//SetBackGroundColor(25.5, 63.75, 127.5);	// �w�i�F
	SetBackGroundColor(0, 0, 0);	// �w�i�F
	NewEngineInit();	// �G���W���̏�����
	TestScene testScene;

	testScene.Load();			// �Q�[�����̃��[�h����
	testScene.Init();	// �Q�[�����̏���������

	frameRate->Initialize(60);

	// �Q�[�����[�v
	while (true)
	{
		////////////////////////////////////////////////////
		// ----------- ��������X�V�������L�q ----------- //
		////////////////////////////////////////////////////
		NewEngineUpda();	// �G���W���̍X�V����
		testScene.Update();			// �Q�[�����̍X�V����
		////////////////////////////////////////////////////
		// ---------- �����܂łɍX�V�������L�q ---------- //
		////////////////////////////////////////////////////
		NewEnginePreDraw();		// �G���W���`��O����
		////////////////////////////////////////////////////
		// ----------- ��������`�揈�����L�q ----------- //
		////////////////////////////////////////////////////
		NewEneineDraw();		// �G���W���̕`�揈��
		testScene.DrawFrontSprite();
		testScene.DrawModel();
		testScene.DrawBackSprite();
		////////////////////////////////////////////////////
		// ---------- �����܂łɕ`�揈�����L�q ---------- //
		////////////////////////////////////////////////////
		NewEnginePostDraw();	// �G���W���`��㏈��

		// X�{�^����������ESC�L�[�ŃQ�[�����[�v�𔲂���
		if (ProcessMessage() || Key::GetKey(DIK_ESCAPE))
		{
			break;
		}

		frameRate->Update();
	}

	NewEngineEnd();	// �G���W���̃G���h����

	// ����I��
	return 0;
}