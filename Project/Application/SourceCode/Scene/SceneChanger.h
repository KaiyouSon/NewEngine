#pragma once
#include "NewEngine.h"

template<typename T> class Singleton;

class SceneChanger : public Singleton<SceneChanger>
{
private:
	enum class ChangeStep
	{
		In,
		Out,
		End
	};

private:
	std::unique_ptr<Sprite> mSprite;
	Easing mMoveEase;
	ChangeStep mChangeStep;

	bool mIsSceneChanging;
	bool mIsChange;

private:
	bool mIsEaseTitleBGM;
	bool mIsEaseGameBGM;

private:
	void Init();
public:
	SceneChanger();
	void Update();
	void Draw();

	void StartSceneChange();
	void SetisEaseTitleBGM(const bool isEaseTitleBGM);
	void SetisEaseGameBGM(const bool isEaseGameBGM);

public:
	inline bool GetisChange() { return mIsChange; }
	inline void SetisChange(bool mIsChange) { this->mIsChange = mIsChange; }
	inline bool GetisSceneChanging() { return mIsSceneChanging; }

private:
	friend Singleton<SceneChanger>;
};
