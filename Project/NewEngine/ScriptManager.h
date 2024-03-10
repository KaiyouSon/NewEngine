#pragma once
#include "StandardLib.h"
#include "Singleton.h"

template<typename T> class Singleton;

class ScriptManager : public Singleton<ScriptManager>
{
private:
	std::unordered_map<std::string, std::shared_ptr<Script>> mScriptMap;

public:
	void Register();
	void RegisterScript(std::shared_ptr<Script>& script, const std::string& tag);

public:
	Script* GetScript(const std::string& tag);
	//std::shared_ptr<Script> CreateScript(const std::string tag);

private:
	friend Singleton<ScriptManager>;

};