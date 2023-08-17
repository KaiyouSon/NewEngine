#pragma once
#include "Coffin.h"
#include "SkyIsland.h"
#include "Tree.h"
#include "RespawnPoint.h" 

struct FieldData
{
	std::vector<std::unique_ptr<Coffin>> coffins;
	std::vector<std::unique_ptr<SkyIsland>> skyIslands;
	std::vector<std::unique_ptr<Tree>> trees;
	std::vector<std::unique_ptr<RespawnPoint>> respawnPoints;
};