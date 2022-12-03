#include "ObjectManager.h"
#include <fstream>
#include <sstream>
#include <stdlib.h>
using namespace std;

void ObjectManager::CreateObject(const int& modelType, const ModelData& modelData)
{
	switch (modelType)
	{
	case CubeObj:
	{
		int num = 0;
		unique_ptr<Object3D> cube = move(make_unique<Object3D>());
		cube->componentManager->GetComponent<ModelData>()->SetModelData(*modelDataList->GetModelData("Cube"));
		cube->Initialize();
		cube->SetName("Cube");
		cube->SetObjectType(CubeObj);
		for (const auto& tmep : list)
		{
			if (tmep->GetName() == cube->GetName())
			{
				num++;
				cube->SetName("Cube" + to_string(num));
			}
		}
		list.push_back(move(cube));
		break;
	}

	case SphereObj:
	{
		int num = 0;
		unique_ptr<Object3D> sphere = move(make_unique<Object3D>());
		sphere->componentManager->GetComponent<ModelData>()->SetModelData(*modelDataList->GetModelData("Sphere"));
		sphere->Initialize();
		sphere->SetName("Sphere");
		sphere->SetObjectType(SphereObj);
		sphere->colliderManager->Add(move(make_unique<SphereCollider>()));
		for (const auto& tmep : list)
		{
			if (tmep->GetName() == sphere->GetName())
			{
				num++;
				sphere->SetName("Sphere" + to_string(num));
			}
		}
		list.push_back(move(sphere));
		break;
	}

	case MonkeyObj:
	{
		int num = 0;
		unique_ptr<Object3D> monkey = move(make_unique<Object3D>());
		monkey->componentManager->GetComponent<ModelData>()->SetModelData(*modelDataList->GetModelData("Monkey"));
		monkey->Initialize();
		monkey->SetName("Monkey");
		monkey->SetObjectType(MonkeyObj);
		for (const auto& tmep : list)
		{
			if (tmep->GetName() == monkey->GetName())
			{
				num++;
				monkey->SetName("Monkey" + to_string(num));
			}
		}
		list.push_back(move(monkey));
		break;
	}

	case MeshObj:
	{
		int num = 0;
		unique_ptr<Object3D> mesh = move(make_unique<Object3D>());
		mesh->componentManager->GetComponent<ModelData>()->SetModelData(*modelDataList->GetModelData("Mesh"));
		mesh->Initialize();
		mesh->SetName("Mesh");
		mesh->SetObjectType(MeshObj);
		for (const auto& tmep : list)
		{
			if (tmep->GetName() == mesh->GetName())
			{
				num++;
				mesh->SetName("Mesh" + to_string(num));
			}
		}
		list.push_back(move(mesh));
		break;
	}

	case ModelObj:
	{
		int num = 0;
		unique_ptr<Object3D> model = move(make_unique<Object3D>());
		model->componentManager->GetComponent<ModelData>()->SetModelData(modelData);
		model->Initialize();
		model->SetName("Model");
		model->SetObjectType(ModelObj);
		for (const auto& tmep : list)
		{
			if (tmep->GetName() == model->GetName())
			{
				num++;
				model->SetName("Model" + to_string(num));
			}
		}
		list.push_back(move(model));
		break;
	}

	case SpriteObj:
	{
		int num = 0;
		unique_ptr<Sprite> sprite = move(make_unique<Sprite>());
		sprite->Initialize();
		sprite->SetName("Sprite");
		sprite->SetObjectType(SpriteObj);
		for (const auto& tmep : list)
		{
			if (tmep->GetName() == sprite->GetName())
			{
				num++;
				sprite->SetName("Sprite" + to_string(num));
			}
		}
		list.push_back(move(sprite));
		break;
	}

	case ParticleEmitter:
	{
		int num = 0;
		unique_ptr<Emitter> emitter = move(make_unique<Emitter>());
		emitter->Initialize();
		emitter->SetName("ParticleEmitter");
		emitter->SetObjectType(ParticleEmitter);
		for (const auto& current : list)
		{
			if (current->GetName() == emitter->GetName())
			{
				num++;
				emitter->SetName("ParticleEmitter" + to_string(num));
			}
		}
		list.push_back(move(emitter));
		break;
	}
	}
}
void ObjectManager::DestroyObject(const string& name)
{
	// ŒŸõ
	for (const auto& temp : list)
	{
		if (temp->GetName() == name)
		{
			list.remove(temp);
			break;
		}
	}
}

GameObject* ObjectManager::GetGameObject(const string& name)
{
	for (const auto& temp : list)
	{
		if (temp->GetName() == name)
		{
			return temp.get();
		}
	}

	return nullptr;
}

// ‚¢‚¸‚ê‚±‚Á‚¿‚É‘‚«Š·‚¦‚é
GameObject& ObjectManager::GetGameObject2(const std::string& name)
{
	for (const auto& temp : list)
	{
		if (temp->GetName() == name)
		{
			return *temp;
		}
	}
}

void ObjectManager::Update()
{
	for (const auto& temp : list)
	{
		temp->Update();
	}
}
void ObjectManager::Draw3D()
{
	for (const auto& current : list)
	{
		if (current->GetisShow() == true &&(
			current->GetObjectType() == CubeObj ||
			current->GetObjectType() == SphereObj ||
			current->GetObjectType() == MonkeyObj ||
			current->GetObjectType() == MeshObj ||
			current->GetObjectType() == ModelObj ||
			current->GetObjectType() == ParticleEmitter))
		{
			current->Draw();
		}
	}
}
void ObjectManager::Draw2DToBack()
{
	for (const auto& temp : list)
	{
		if (temp->GetisShow() == true &&
			temp->GetLayer() == false &&
			temp->GetObjectType() == SpriteObj)
		{
			temp->Draw();
		}
	}

	//for (int i = 0; i < gameObjectList.size(); i++)
	//{
	//	if (gameObjectList[i]->GetisShow() == true &&
	//		gameObjectList[i]->GetObjectType() == SpriteObj)
	//	{
	//		if (dynamic_cast<Sprite*>(gameObjectList[i])->GetLayer() == false)
	//			dynamic_cast<Sprite*>(gameObjectList[i])->Draw();
	//	}
	//}
}
void ObjectManager::Draw2DToForward()
{
	for (const auto& temp : list)
	{
		if (temp->GetisShow() == true &&
			temp->GetLayer() == true &&
			temp->GetObjectType() == SpriteObj)
		{
			temp->Draw();
		}
	}

	//for (int i = 0; i < gameObjectList.size(); i++)
	//{
	//	if (gameObjectList[i]->GetisShow() == true &&
	//		gameObjectList[i]->GetObjectType() == SpriteType)
	//	{
	//		if (dynamic_cast<Sprite*>(gameObjectList[i])->GetLayer() == true)
	//			dynamic_cast<Sprite*>(gameObjectList[i])->Draw();
	//	}
	//}
}