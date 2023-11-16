#include "FieldDataManager.h"

std::unordered_map<std::string, std::unique_ptr<FieldData>> FieldDataManager::sFieldDataMap;

FieldData* FieldDataManager::Load(const std::string filename, const std::string tag)
{
	// ファイルを開く
	std::ifstream file;

	std::string directoryPath = "Application/Data/FieldData/";
	std::string format = ".json";
	std::string fullPath = directoryPath + filename + format;

	// ファイルを開く
	file.open(fullPath);
	// ファイルが開けない場合はアサーションエラー
	if (file.fail())
	{
		assert(0);
	}

	// JSONをデシリアライズ
	nlohmann::json deserialized;

	// デシリアライズ
	file >> deserialized;

	// ルート要素はオブジェクトであることを確認
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	// "name"フィールドを取得
	std::string name = deserialized["name"].get<std::string>();
	// フィールドの値が"scene"であることを確認
	assert(name.compare("scene") == 0);

	std::unique_ptr<FieldData> fieldData = std::make_unique<FieldData>();

	// "objects"フィールドの各オブジェクトを処理
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
			else if (object["obj_name"] == "WallGate")
			{
				LoadGateData(fieldData.get(), object);
			}
			else if (object["obj_name"] == "VolumetricFog")
			{
				LoadVolumetricFogData(fieldData.get(), object);
			}
			else if (object["obj_name"] == "Sun")
			{
				LoadSunData(fieldData.get(), object);
			}
			else if (object["obj_name"] == "AirCollider")
			{
				LoadAirColliderData(fieldData.get(), object);
			}
		}
	}

	// タグが重複した場合
	if (sFieldDataMap.find(tag) != sFieldDataMap.end())
	{
		// 古い方のタグを削除する
		sFieldDataMap.erase(tag);
	}

	// mapに挿入
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
	// 棺の位置、スケール、角度情報を取得
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

	// 子要素の処理
	if (jsonObj.contains("children"))
	{
		nlohmann::json children = jsonObj["children"];
		for (const auto& child : children)
		{
			nlohmann::json ctransform = child["transform"];

			if (!jsonObj.contains("obj_name"))
			{
				continue;
			}
			else if (child["obj_name"] == "CoffinBottom")
			{
				Vec3 bottomPos =
				{
					(float)ctransform["translation"][0],
					(float)ctransform["translation"][1],
					(float)ctransform["translation"][2],
				};
				coffin->SetBottomPos(bottomPos);

				Vec3 bottomAngle =
				{
					(float)ctransform["rotation"][0],
					(float)ctransform["rotation"][1],
					(float)ctransform["rotation"][2],
				};
				coffin->SetBottomRot(Radian(bottomAngle));

				if (child.contains("collider"))
				{
					nlohmann::json collider = child["collider"];

					if (collider["type"] == "Box")
					{
						Vec3 childPos =
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

						coffin->SetBottomCollider(CubeCollider(childPos, size));
					}
				}
			}
			else if (child["obj_name"] == "CoffinTop")
			{
				Vec3 topPos =
				{
					(float)ctransform["translation"][0],
					(float)ctransform["translation"][1],
					(float)ctransform["translation"][2],
				};
				coffin->SetTopPos(topPos);

				Vec3 topAngle =
				{
					(float)ctransform["rotation"][0],
					(float)ctransform["rotation"][1],
					(float)ctransform["rotation"][2],
				};
				coffin->SetTopRot(Radian(topAngle));
			}
		}
	}
	data->coffins.push_back(std::move(coffin));
}
void FieldDataManager::LoadSkyIslandData(FieldData* data, nlohmann::json jsonObj)
{
	std::unique_ptr<SkyIsland> skyIsland = std::make_unique<SkyIsland>();
	// 空島の位置、スケール、角度情報を取得
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
	// 木の位置、スケール、角度情報を取得
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
		Vec3 colliderPos =
		{
			collider["center"][0],
			collider["center"][1],
			collider["center"][2],
		};
		float radius = collider["radius"];
		tree->SetCollider(SphereCollider(colliderPos, radius));
	}

	data->trees.push_back(std::move(tree));
}
void FieldDataManager::LoadRespawnPointData(FieldData* data, nlohmann::json jsonObj)
{
	std::unique_ptr<RespawnPoint> respawnPoint = std::make_unique<RespawnPoint>();
	// リスポーン地点の位置、スケール、角度情報を取得
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
	// 雑草の位置、スケール、角度情報を取得
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
	// 城壁の位置、スケール、角度情報を取得
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

	// Colliderある場合
	if (jsonObj.contains("collider"))
	{
		nlohmann::json collider = jsonObj["collider"];
		if (collider["type"] == "Capsule")
		{
			Vec3 start =
			{
				collider["start"][0],
				collider["start"][1],
				collider["start"][2],
			};
			Vec3 end =
			{
				collider["end"][0],
				collider["end"][1],
				collider["end"][2],
			};
			float radius = collider["radius"];
			wall->SetCollider(CapsuleCollider(start, end, radius));
		}
	}

	data->walls.push_back(std::move(wall));
}
void FieldDataManager::LoadGateData(FieldData* data, nlohmann::json jsonObj)
{
	std::unique_ptr<Gate> gate = std::make_unique<Gate>();
	// ゲートの位置、スケール、角度情報を取得
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
	gate->SetCenterPos(pos);

	Transform parent = Transform(pos, scale, Radian(angle));
	parent.Update();

	// Collider情報の処理
	if (jsonObj.contains("collider"))
	{
		nlohmann::json collider = jsonObj["collider"];
		if (collider["type"] == "Sphere")
		{
			Vec3 center =
			{
				collider["center"][0],
				collider["center"][1],
				collider["center"][2],
			};
			float radius = collider["radius"];
			gate->SetNegotiationCollider(SphereCollider(center, radius));
		}
	}

	// 子要素の処理
	if (jsonObj.contains("children"))
	{
		nlohmann::json children = jsonObj["children"];
		for (const auto& child : children)
		{
			nlohmann::json ctransform = child["transform"];

			if (!jsonObj.contains("obj_name"))
			{
				continue;
			}
			else if (child["obj_name"] == "GateLeft")
			{
				Vec3 cpos =
				{
					(float)ctransform["translation"][0],
					(float)ctransform["translation"][1],
					(float)ctransform["translation"][2],
				};
				Vec3 cscale =
				{
					(float)ctransform["scaling"][0],
					(float)ctransform["scaling"][1],
					(float)ctransform["scaling"][2],
				};
				Vec3 cangle =
				{
					(float)ctransform["rotation"][0],
					(float)ctransform["rotation"][1],
					(float)ctransform["rotation"][2],
				};

				cpos = Vec3MulMat4(cpos, parent.GetWorldMat());
				gate->SetLeftTransform(Transform(cpos, cscale, Radian(cangle)));
			}
			else if (child["obj_name"] == "GateRight")
			{
				Vec3 cpos =
				{
					(float)ctransform["translation"][0],
					(float)ctransform["translation"][1],
					(float)ctransform["translation"][2],
				};
				Vec3 cscale =
				{
					(float)ctransform["scaling"][0],
					(float)ctransform["scaling"][1],
					(float)ctransform["scaling"][2],
				};
				Vec3 cangle =
				{
					(float)ctransform["rotation"][0],
					(float)ctransform["rotation"][1],
					(float)ctransform["rotation"][2],
				};

				cpos = Vec3MulMat4(cpos, parent.GetWorldMat());
				gate->SetRightTransform(Transform(cpos, cscale, Radian(cangle)));
			}
		}
	}

	data->gates.push_back(std::move(gate));
}
void FieldDataManager::LoadVolumetricFogData(FieldData* data, nlohmann::json jsonObj)
{
	std::unique_ptr<VolumetricFog> volumetricFog = std::make_unique<VolumetricFog>();
	// ゲートの位置、スケール、角度情報を取得
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
	volumetricFog->pos = pos;
	volumetricFog->scale = scale;
	volumetricFog->rot = Radian(angle);
	volumetricFog->SetTexture(TextureManager::GetVolumeTexture("VolumeTexture1"));

	// フォグのパラメーター
	if (jsonObj.contains("fog_param"))
	{
		nlohmann::json fogParam = jsonObj["fog_param"];
		volumetricFog->fogParam.stepCount = fogParam["step_count"];
		volumetricFog->fogParam.stepLength = fogParam["step_length"];
		volumetricFog->fogParam.dencity = fogParam["dencity"];

		Color color =
		{
			fogParam["color"][0],
			fogParam["color"][1],
			fogParam["color"][2],
			fogParam["color"][3],
		};
		volumetricFog->fogParam.fogColor = color;

		Color colorRate =
		{
			fogParam["color_rate"][0],
			fogParam["color_rate"][1],
			fogParam["color_rate"][2],
			fogParam["color_rate"][3],
		};
		volumetricFog->fogParam.fogColorRate = colorRate;
	}

	if (jsonObj.contains("uv_parameter"))
	{
		nlohmann::json uvParameter = jsonObj["uv_parameter"];

		Vec3 tiling =
		{
			uvParameter["tiling"][0],
			uvParameter["tiling"][1],
			uvParameter["tiling"][2],
		};
		volumetricFog->tiling = tiling;

		Vec3 offset =
		{
			uvParameter["offset"][0],
			uvParameter["offset"][1],
			uvParameter["offset"][2],
		};
		volumetricFog->offset = offset;
	}

	data->volumetricFogs.push_back(std::move(volumetricFog));
}
void FieldDataManager::LoadSunData(FieldData* data, nlohmann::json jsonObj)
{
	std::unique_ptr<Sun> sun = std::make_unique<Sun>();
	// ゲートの位置、スケール、角度情報を取得
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
	sun->SetTransform(Transform(pos, scale, Radian(angle)));

	data->suns.push_back(std::move(sun));
}
void FieldDataManager::LoadAirColliderData(FieldData* data, nlohmann::json jsonObj)
{
	std::unique_ptr<AirCollider> airCollider = std::make_unique<AirCollider>();

	// Colliderある場合
	if (jsonObj.contains("collider"))
	{
		nlohmann::json collider = jsonObj["collider"];
		if (collider["type"] == "Capsule")
		{
			Vec3 start =
			{
				collider["start"][0],
				collider["start"][1],
				collider["start"][2],
			};
			Vec3 end =
			{
				collider["end"][0],
				collider["end"][1],
				collider["end"][2],
			};
			float radius = collider["radius"];
			airCollider->SetCollider(CapsuleCollider(start, end, radius));
		}
	}

	data->airColliders.push_back(std::move(airCollider));
}
