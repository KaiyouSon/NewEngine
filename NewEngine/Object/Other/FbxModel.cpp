#include "FbxModel.h"

void FbxAnimetion::Init()
{
	frameTime.SetTime(0, 0, 0, 1, 0, FbxTime::EMode::eFrames60);
}

void FbxAnimetion::Update()
{
	if (isPlay == true)
	{
		// 1フレーム進める
		currentTime += frameTime;
		// 最後まで再生したら先頭に戻す
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

	// 0番のアニメーション取得
	FbxAnimStack* animStack = fbxScene.GetSrcObject<FbxAnimStack>(0);
	// アニメーションの名前所得
	const char* animStackName = animStack->GetName();
	// アニメーションの時間情報
	FbxTakeInfo* takeInfo = fbxScene.GetTakeInfo(animStackName);

	// 開始時間
	startTime = takeInfo->mLocalTimeSpan.GetStart();
	// 終了時間
	endTime = takeInfo->mLocalTimeSpan.GetStop();
	// 開始時間に合わせる
	currentTime = startTime;
	// 再生中状態にする
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
