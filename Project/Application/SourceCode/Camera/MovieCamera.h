#pragma once
#include "NewEngine.h"
#include "ICamera.h"
#include "MovieEvent.h"

class MovieCamera : public ICamera
{
private:
	static MovieEvent* sMovieEvent;
	Vec3 mFrontVec;

public:
	MovieCamera();
	void Init(Player* player) override;
	void Update() override;

public:
	static void SetMovieEvent(MovieEvent* movieEvent);
};
