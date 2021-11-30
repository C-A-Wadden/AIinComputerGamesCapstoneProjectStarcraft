#pragma once

#include <BWAPI.h>

class BaseManager
{
public:

	BaseManager();

	bool BaseManager::build(BWAPI::UnitType building, BWAPI::Unit builder, BWAPI::TilePosition pos, bool queue);
	bool BaseManager::manageSupply(BWAPI::Unit builder);

	std::vector<BWAPI::UnitType> buildOrder;

	//bool BaseManager::expand(BWAPI::TilePosition location);
	//bool BaseManager::cancelBuilding(BWAPI::UnitType building);

};
