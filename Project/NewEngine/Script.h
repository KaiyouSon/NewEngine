#pragma once
#include "StandardLib.h"

class Script
{
protected:
	std::string tag;
	GameObject* mGameObject;

public:
	Script() {}
	virtual ~Script() {}
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual std::shared_ptr<Script> CreateInstence() = 0;

public:
	void SetGameObject(GameObject* gameObject);

public:
	std::string GetTag();
};

