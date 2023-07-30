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
	std::unique_ptr<Sprite> sprite;
	Easing moveEase;
	ChangeStep changeStep_;

	bool isSceneChanging;
	bool isChange;

private:
	bool isEaseTitleBGM_;
	bool isEaseGameBGM_;

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
	inline bool GetisChange() { return isChange; }
	inline void SetisChange(bool isChange) { this->isChange = isChange; }
	inline bool GetisSceneChanging() { return isSceneChanging; }

private:
	friend Singleton<SceneChanger>;
};
