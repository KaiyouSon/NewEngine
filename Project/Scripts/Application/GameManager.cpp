#include "GameManager.h"

void ScriptRegistrar::RegisterScript()
{
	static std::shared_ptr<Script> instance = std::make_unique<AfterImager>();
	ScriptManager::GetInstance()->RegisterScript(instance, instance->GetTag());
}