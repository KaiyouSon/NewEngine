#include "ModelData.h"
#include "DrawManager.h"
using namespace std;

ModelDataManager::ModelDataManager()
{
	list.push_back(LoadModelData("NewEngine/BasicModel/cube.obj"));
	list.back()->SetTag("Cube");
	list.back()->SetDataType(DefaultModel);

	list.push_back(LoadModelData("NewEngine/BasicModel/sphere.obj"));
	list.back()->SetTag("Sphere");
	list.back()->SetDataType(DefaultModel);

	list.push_back(LoadModelData("NewEngine/BasicModel/monkey.obj"));
	list.back()->SetTag("Monkey");
	list.back()->SetDataType(DefaultModel);

	list.push_back(LoadModelData("NewEngine/BasicModel/mesh.obj"));
	list.back()->SetTag("Mesh");
	list.back()->SetDataType(DefaultModel);
}

void ModelDataManager::Add(unique_ptr<ModelData>&& modelData, const string& tag)
{
	list.push_back(move(modelData));
	list.back()->SetTag(tag);
	list.back()->SetDataType(LoadModel);
}

ModelData* ModelDataManager::GetModelData(const string& tag)
{
	for (const auto& temp : list)
	{
		if (temp->GetTag() == tag)
		{
			return temp.get();
		}
	}
}

void ModelDataManager::DestroyModelData(const string& tag)
{
	for (const auto& temp : list)
	{
		if (temp->GetTag() == tag)
		{
			list.remove(temp);
			break;
		}
	}
}

unique_ptr<ModelDataManager> modelDataList = move(make_unique<ModelDataManager>());