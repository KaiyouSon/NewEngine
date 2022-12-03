#pragma once
#include "ObjectManager.h"
#include <string>
#include <vector>
#include <memory>
#include <list>

class Scene
{
private:	// �ÓI�ϐ�
	static int numberIndex;

private:
	std::string name;	// �V�[���̖��O
	int index;			// �V�[���̃C���f�b�N�X
	std::unique_ptr<ObjectManager> objManager;

public:
	Scene();

public: // �Z�b�^�[
	inline void SetName(const std::string& name) { this->name = name; }
	inline void SetIndex(const int& index) { this->index = index; }

public: // �Q�b�^�[
	inline std::string GetName() { return name; }
	inline int GetIndex() { return index; }
	inline ObjectManager* GetObjManager() { return objManager.get(); }
};

class SceneManager
{
private:
	int current;	// ���݂̃V�[��

public:
	std::list<std::unique_ptr<Scene>> list;

public:
	SceneManager();
	~SceneManager();
	void Update();
	void Draw();

	void AddScene(const std::string& name);			// �V�[���̒ǉ�
	void DestroyScene(const std::string& name);		// �V�[���̍폜

public: // �Z�b�^�[
	inline void SetCurrent(const int& current) { this->current = current; }

public: // �Q�b�^�[
	inline int GetCurrent() { return current; }
	//inline const std::list<std::unique_ptr<Scene>>& GetList() { return list; };
};

extern std::unique_ptr<SceneManager> sceneManager;