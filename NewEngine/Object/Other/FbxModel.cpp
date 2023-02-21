#include "FbxModel.h"

void FbxAnimetion::Init()
{
	frameTime.SetTime(0, 0, 0, 1, 0, FbxTime::EMode::eFrames60);
}

void FbxAnimetion::Update()
{
	if (isPlay == true)
	{
		// 1�t���[���i�߂�
		currentTime += frameTime;
		// �Ō�܂ōĐ�������擪�ɖ߂�
		if (currentTime > endTime)
		{
			isPlay = false;
			//currentTime = startTime;
		}
	}
}

void FbxAnimetion::Play(const FbxScene& fbxScene)
{
	if (isPlay == true)
	{
		return;
	}

	// 0�Ԃ̃A�j���[�V�����擾
	FbxAnimStack* animStack = fbxScene.GetSrcObject<FbxAnimStack>(0);
	// �A�j���[�V�����̖��O����
	const char* animStackName = animStack->GetName();
	// �A�j���[�V�����̎��ԏ��
	FbxTakeInfo* takeInfo = fbxScene.GetTakeInfo(animStackName);

	// �J�n����
	startTime = takeInfo->mLocalTimeSpan.GetStart();
	// �I������
	endTime = takeInfo->mLocalTimeSpan.GetStop();
	// �J�n���Ԃɍ��킹��
	currentTime = startTime;
	// �Đ�����Ԃɂ���
	isPlay = true;
}

FbxModel::FbxModel()
{
	modelType = "FBX";
	fbxAnimetion.Init();
}

void FbxModel::PlayAnimetion()
{
	fbxAnimetion.Play(*fbxScene);
	fbxAnimetion.Update();
}
