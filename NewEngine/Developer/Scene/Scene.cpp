#include "Scene.h"
using namespace std;

// --- �V�[���֘A ------------------------------------- // 
int Scene::numberIndex = 0;
Scene::Scene() :
	objManager(make_unique<ObjectManager>())
{
	index = numberIndex;
	numberIndex++;
}

// --- �V�[���}�l�W���[�֘A ------------------------------- //
SceneManager::SceneManager() : current(0)
{
	static int number = 0;
	list.push_back(move(make_unique<Scene>()));
	list.back()->SetName("Default");
	list.back()->SetIndex(number);
	number++;
}
SceneManager::~SceneManager()
{
}

void SceneManager::Update()
{
	// �C�e���[�^�[���擾
	decltype(list)::iterator it = next(list.begin(), current);

	it->get()->GetObjManager()->Update();
}
void SceneManager::Draw()
{
	// �C�e���[�^�[���擾
	decltype(list)::iterator it = next(list.begin(), current);

	it->get()->GetObjManager()->Draw2DToBack();
	it->get()->GetObjManager()->Draw3D();
	it->get()->GetObjManager()->Draw2DToForward();
}

void SceneManager::AddScene(const string& name)
{
	if (name.empty() == true) return;

	// ���O���d�����ĂȂ���������
	for (const auto& temp : list)
	{
		if (temp->GetName() == name)
		{
			return;
		}
	}

	unique_ptr<Scene> scene = make_unique<Scene>();
	scene->SetName(name);
	list.push_back(move(scene));
}
void SceneManager::DestroyScene(const string& name)
{
	// ����
	for (const auto& temp : list)
	{
		if (temp->GetName() == name)
		{
			list.remove(temp);
			break;
		}
	}
}

unique_ptr<SceneManager> sceneManager = make_unique<SceneManager>();