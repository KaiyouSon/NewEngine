#pragma once
#include "Coffin.h"
#include "Wall.h"
#include "Gate.h"

#include "SkyIsland.h"
#include "Tree.h"
#include "RespawnPoint.h" 
#include "Weed.h"
#include "VolumetricFog.h"
#include "Sun.h"
#include "AirCollider.h"
#include "Tower.h"
#include "Bridge.h"

#include "IFieldObject.h"

// フィールドデータの構造体
struct FieldData
{
	std::vector<std::unique_ptr<IFieldObject>> mFieldObjects;

	std::vector<std::unique_ptr<SkyIsland>> skyIslands;
	std::vector<std::unique_ptr<Tree>> trees;
	std::vector<std::unique_ptr<RespawnPoint>> respawnPoints;
	std::vector<std::unique_ptr<Weed>> weeds;
	std::vector<std::unique_ptr<VolumetricFog>> volumetricFogs;
	std::vector<std::unique_ptr<Sun>> suns;
	std::vector<std::unique_ptr<AirCollider>> airColliders;
	std::vector<std::unique_ptr<Tower>> towers;
	std::vector<std::unique_ptr<Bridge>> bridges;
};
