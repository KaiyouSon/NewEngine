#pragma once
#include "Coffin.h"
#include "SkyIsland.h"
#include "Tree.h"
#include "RespawnPoint.h" 
#include "Weed.h"
#include "Wall.h"
#include "Gate.h"

struct FieldData
{
	std::vector<std::unique_ptr<Coffin>> coffins;
	std::vector<std::unique_ptr<SkyIsland>> skyIslands;
	std::vector<std::unique_ptr<Tree>> trees;
	std::vector<std::unique_ptr<RespawnPoint>> respawnPoints;
	std::vector<std::unique_ptr<Weed>> weeds;
	std::vector<std::unique_ptr<Wall>> walls;
	std::vector<std::unique_ptr<Gate>> gates;
};
