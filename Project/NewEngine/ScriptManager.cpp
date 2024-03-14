#include "ScriptManager.h"
#include "AfterImage.h"
#include "GameManager.h"

void ScriptManager::Register()
{
	std::shared_ptr<Script> instance;
	instance = std::make_unique<AfterImage>();
	ScriptManager::GetInstance()->RegisterScript(instance, instance->GetTag());

	instance = std::make_unique<GameManager>();
	ScriptManager::GetInstance()->RegisterScript(instance, instance->GetTag());
}

void ScriptManager::RegisterScript(std::shared_ptr<Script>& script, const std::string& tag)
{
	mScriptMap.insert(std::make_pair(tag, std::move(script)));
}

Script* ScriptManager::GetScript(const std::string& tag)
{
	return mScriptMap[tag].get();
}