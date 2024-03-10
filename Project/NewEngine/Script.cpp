#include "Script.h"

void Script::SetGameObject(GameObject* gameObject)
{
	mGameObject = gameObject;
}

std::string Script::GetTag()
{
	return tag;
}
