#include "FieldDataManager.h"

std::unordered_map<std::string, std::unique_ptr<FieldData>> FieldDataManager::sFieldDataMap;

FieldData* FieldDataManager::Load(const std::string filename, const std::string tag)
{
	// �t�@�C���X�g���[��
	std::ifstream file;

	std::string directoryPath = "Application/Data/FieldData/";
	std::string format = ".json";
	std::string fullPath = directoryPath + filename + format;

	// �t�@�C�����J��
	file.open(fullPath);
	// �t�@�C���I�[�v�����s���`�F�b�Ns
	if (file.fail())
	{
		assert(0);
	}

	// JSON�����񂩂�𓀂����f�[�^
	nlohmann::json deserialized;

	// ��
	file >> deserialized;

	// ���������x���G�f�B�^�[�f�[�^�t�@�C�����`�F�b�N
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	// "name"�𕶎���Ƃ��Ď擾
	std::string name = deserialized["name"].get<std::string>();
	// ���������x���G�f�B�^�[�f�[�^�t�@�C�����`�F�b�N
	assert(name.compare("scene") == 0);

	std::unique_ptr<FieldData> fieldData = std::make_unique<FieldData>();

	// "objects"�̑S�I�u�W�F�N�g�𑖍�
	for (nlohmann::json& object : deserialized["object"])
	{
		if (object["obj_name"] == nullptr)
		{
			continue;
		}
		else
		{
			if (object["obj_name"] == "Coffin")
			{
				LoadCoffinData(fieldData.get(), object);
			}
			else if (
				object["obj_name"] == "SkyIsland1" ||
				object["obj_name"] == "SkyIsland2")
			{
				LoadSkyIslandData(fieldData.get(), object);
			}
			else if (object["obj_name"] == "Tree")
			{
				LoadTreeData(fieldData.get(), object);
			}
			else if (object["obj_name"] == "Weed")
			{
				LoadWeedData(fieldData.get(), object);
			}
			else if (object["obj_name"] == "RespawnPoint")
			{
				LoadRespawnPointData(fieldData.get(), object);
			}
			else if (
				object["obj_name"] == "MainWall" ||
				object["obj_name"] == "Wall1" ||
				object["obj_name"] == "Wall2")
			{
				LoadWallData(fieldData.get(), object);
			}
		}
	}

	// map�Ɋi�[
	sFieldDataMap.insert(std::make_pair(tag, std::move(fieldData)));
	return sFieldDataMap[tag].get();
}
FieldData* FieldDataManager::GetFieldData(const std::string tag)
{
	return sFieldDataMap[tag].get();
}

void FieldDataManager::LoadCoffinData(FieldData* data, nlohmann::json jsonObj)
{
	std::unique_ptr<Coffin> coffin = std::make_unique<Coffin>();
	// �g�����X�t�H�[���̃p�����[�^�ǂݍ���
	nlohmann::json transform = jsonObj["transform"];
	Vec3 pos =
	{
		(float)transform["translation"][0],
		(float)transform["translation"][1],
		(float)transform["translation"][2],
	};
	Vec3 scale =
	{
		(float)transform["scaling"][0],
		(float)transform["scaling"][1],
		(float)transform["scaling"][2],
	};
	Vec3 angle =
	{
		(float)transform["rotation"][0],
		(float)transform["rotation"][1],
		(float)transform["rotation"][2],
	};
	coffin->SetParent(Transform(pos, scale, Radian(angle)));

	// �q�������
	if (jsonObj.contains("children"))
	{
		nlohmann::json children = jsonObj["children"];
		for (const auto& child : children)
		{
			nlohmann::json transform = child["transform"];

			if (!jsonObj.contains("obj_name"))
			{
				continue;
			}
			else if (child["obj_name"] == "CoffinBottom")
			{
				Vec3 pos =
				{
					(float)transform["translation"][0],
					(float)transform["translation"][1],
					(float)transform["translation"][2],
				};
				coffin->SetBottomPos(pos);

				Vec3 angle =
				{
					(float)transform["rotation"][0],
					(float)transform["rotation"][1],
					(float)transform["rotation"][2],
				};
				coffin->SetBottomRot(Radian(angle));

				if (child.contains("collider"))
				{
					nlohmann::json collider = child["collider"];

					if (collider["type"] == "Box")
					{
						Vec3 pos =
						{
							collider["center"][0],
							collider["center"][1],
							collider["center"][2],
						};
						Vec3 size =
						{
							collider["size"][0],
							collider["size"][1],
							collider["size"][2],
						};

						coffin->SetBottomCollider(CubeCollider(pos, size));
					}
				}
			}
			else if (child["obj_name"] == "CoffinTop")
			{
				Vec3 pos =
				{
					(float)transform["translation"][0],
					(float)transform["translation"][1],
					(float)transform["translation"][2],
				};
				coffin->SetTopPos(pos);

				Vec3 angle =
				{
					(float)transform["rotation"][0],
					(float)transform["rotation"][1],
					(float)transform["rotation"][2],
				};
				coffin->SetTopRot(Radian(angle));
			}
		}
	}
	data->coffins.push_back(std::move(coffin));
}
void FieldDataManager::LoadSkyIslandData(FieldData* data, nlohmann::json jsonObj)
{
	std::unique_ptr<SkyIsland> skyIsland = std::make_unique<SkyIsland>();
	// �g�����X�t�H�[���̃p�����[�^�ǂݍ���
	nlohmann::json transform = jsonObj["transform"];
	Vec3 pos =
	{
		(float)transform["translation"][0],
		(float)transform["translation"][1],
		(float)transform["translation"][2],
	};
	Vec3 scale =
	{
		(float)transform["scaling"][0],
		(float)transform["scaling"][1],
		(float)transform["scaling"][2],
	};
	Vec3 angle =
	{
		(float)transform["rotation"][0],
		(float)transform["rotation"][1],
		(float)transform["rotation"][2],
	};
	skyIsland->SetParent(Transform(pos, scale, Radian(angle)));
	skyIsland->SetModel(ModelManager::GetModel(jsonObj["obj_name"]));

	data->skyIslands.push_back(std::move(skyIsland));
}
void FieldDataManager::LoadTreeData(FieldData* data, nlohmann::json jsonObj)
{
	std::unique_ptr<Tree> tree = std::make_unique<Tree>();
	// �g�����X�t�H�[���̃p�����[�^�ǂݍ���
	nlohmann::json transform = jsonObj["transform"];
	Vec3 pos =
	{
		(float)transform["translation"][0],
		(float)transform["translation"][1],
		(float)transform["translation"][2],
	};
	Vec3 scale =
	{
		(float)transform["scaling"][0],
		(float)transform["scaling"][1],
		(float)transform["scaling"][2],
	};
	Vec3 angle =
	{
		(float)transform["rotation"][0],
		(float)transform["rotation"][1],
		(float)transform["rotation"][2],
	};
	tree->SetParent(Transform(pos, scale, Radian(angle)));

	nlohmann::json collider = jsonObj["collider"];
	if (collider["type"] == "Sphere")
	{
		Vec3 pos =
		{
			collider["center"][0],
			collider["center"][1],
			collider["center"][2],
		};
		float radius = collider["radius"];
		tree->SetCollider(SphereCollider(pos, radius));
	}

	data->trees.push_back(std::move(tree));
}
void FieldDataManager::LoadRespawnPointData(FieldData* data, nlohmann::json jsonObj)
{
	std::unique_ptr<RespawnPoint> respawnPoint = std::make_unique<RespawnPoint>();
	// �g�����X�t�H�[���̃p�����[�^�ǂݍ���
	nlohmann::json transform = jsonObj["transform"];
	Vec3 pos =
	{
		(float)transform["translation"][0],
		(float)transform["translation"][1],
		(float)transform["translation"][2],
	};
	Vec3 scale =
	{
		(float)transform["scaling"][0],
		(float)transform["scaling"][1],
		(float)transform["scaling"][2],
	};
	Vec3 angle =
	{
		(float)transform["rotation"][0],
		(float)transform["rotation"][1],
		(float)transform["rotation"][2],
	};
	respawnPoint->SetParent(Transform(pos, scale, Radian(angle)));

	data->respawnPoints.push_back(std::move(respawnPoint));
}
void FieldDataManager::LoadWeedData(FieldData* data, nlohmann::json jsonObj)
{
	std::unique_ptr<Weed> weed = std::make_unique<Weed>();
	// �g�����X�t�H�[���̃p�����[�^�ǂݍ���
	nlohmann::json transform = jsonObj["transform"];
	Vec3 pos =
	{
		(float)transform["translation"][0],
		(float)transform["translation"][1],
		(float)transform["translation"][2],
	};
	Vec3 scale =
	{
		(float)transform["scaling"][0],
		(float)transform["scaling"][1],
		(float)transform["scaling"][2],
	};
	weed->SetPos(pos);
	weed->SetGenerateSize(Vec2(scale.x, scale.z) / 2);

	data->weeds.push_back(std::move(weed));
}
void FieldDataManager::LoadWallData(FieldData* data, nlohmann::json jsonObj)
{
	std::unique_ptr<Wall> wall = std::make_unique<Wall>();
	// �g�����X�t�H�[���̃p�����[�^�ǂݍ���
	nlohmann::json transform = jsonObj["transform"];
	Vec3 pos =
	{
		(float)transform["translation"][0],
		(float)transform["translation"][1],
		(float)transform["translation"][2],
	};
	Vec3 scale =
	{
		(float)transform["scaling"][0],
		(float)transform["scaling"][1],
		(float)transform["scaling"][2],
	};
	Vec3 angle =
	{
		(float)transform["rotation"][0],
		(float)transform["rotation"][1],
		(float)transform["rotation"][2],
	};
	wall->SetParent(Transform(pos, scale, Radian(angle)));
	wall->SetModel(ModelManager::GetModel(jsonObj["obj_name"]));

	// Collider�������
	if (jsonObj.contains("collider"))
	{
		nlohmann::json collider = jsonObj["collider"];
		//if (collider["type"] == "Sphere")
		//{
		//	Vec3 pos =
		//	{
		//		collider["center"][0],
		//		collider["center"][1],
		//		collider["center"][2],
		//	};
		//	float radius = collider["size"][0];
		//	wall->SetCollider(SphereCollider(pos, radius));
		//}
	}

	data->walls.push_back(std::move(wall));
}
