#pragma once
#include "NewEngine.h"
#include "Coffin.h"

class Player;

class MovieEvent
{
public:
	enum class Step
	{
		None,
		Start,
		Play,
		End,
	};

	enum class PlayStep
	{
		None,
		CameraZoom,
		OpenCoffin,
		End,
	};

private:
	static bool sIsPlayOnce;

private:
	bool mIsStart;
	bool mIsEnd;
	Step mStep;
	PlayStep mPlayStep;
	Camera mCamera;

private:
	// �J�����Y�[���֘A
	Timer mTimer;

private:
	// �������J���֘A
	Easing mEase;
	Vec3 mStartPos;
	Vec3 mEndPos;

private:
	// �C���X�^���X
	Player* mPlayer;
	Coffin* mPlayerCoffin;

private:
	void PlayMovieUpdate();

public:
	void Init();
	void Update();

public:
	void Start();
	void End();

public:
	void SetPlayer(Player* player);
	void SetPlayerCoffin(Coffin* playerCoffin);
	static void SetisPlayOnce(const bool isPlayOnce);

public:
	bool GetisPlaying();
	PlayStep GetPlayStep();
};

