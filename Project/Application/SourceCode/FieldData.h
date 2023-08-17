#pragma once
#include "Coffin.h"
#include "SkyIsland.h"

struct FieldData
{
	std::vector<std::unique_ptr<Coffin>> coffins;
	std::vector<std::unique_ptr<SkyIsland>> skyIslands;

};