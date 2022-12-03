#pragma once
#include "ObjectManager.h"
#include <string>
#include <vector>
#include <memory>
#include <list>

class Scene
{
private:	// 静的変数
	static int numberIndex;

private:
	std::string name;	// シーンの名前
	int index;			// シーンのインデックス
	std::unique_ptr<ObjectManager> objManager;

public:
	Scene();

public: // セッター
	inline void SetName(const std::string& name) { this->name = name; }
	inline void SetIndex(const int& index) { this->index = index; }

public: // ゲッター
	inline std::string GetName() { return name; }
	inline int GetIndex() { return index; }
	inline ObjectManager* GetObjManager() { return objManager.get(); }
};

class SceneManager
{
private:
	int current;	// 現在のシーン

public:
	std::list<std::unique_ptr<Scene>> list;

public:
	SceneManager();
	~SceneManager();
	void Update();
	void Draw();

	void AddScene(const std::string& name);			// シーンの追加
	void DestroyScene(const std::string& name);		// シーンの削除

public: // セッター
	inline void SetCurrent(const int& current) { this->current = current; }

public: // ゲッター
	inline int GetCurrent() { return current; }
	//inline const std::list<std::unique_ptr<Scene>>& GetList() { return list; };
};

extern std::unique_ptr<SceneManager> sceneManager;