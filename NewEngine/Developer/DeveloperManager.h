#pragma once
#include "Singleton.h"

template<typename T> class Singleton;

class DeveloperManager : public Singleton<DeveloperManager>
{
private:
	friend Singleton<DeveloperManager>;
public:
	void Initialize();
	void Update();
	void Draw();
};

extern DeveloperManager* developerManager;