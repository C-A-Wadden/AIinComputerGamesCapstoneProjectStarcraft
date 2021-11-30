#include "BaseManager.h"
#include "Tools.h"


BaseManager::BaseManager()
{

}


bool BaseManager::build(BWAPI::UnitType unit, BWAPI::Unit builder, BWAPI::TilePosition pos, bool queue)
{
	switch (unit)
	{
		case BWAPI::UnitTypes::Protoss_Pylon:
			return Tools::BuildLocation(unit, builder, pos);

		case BWAPI::UnitTypes::Protoss_Gateway:
			return Tools::BuildLocation(unit, builder, pos);

		case BWAPI::UnitTypes::Protoss_Cybernetics_Core:
			return Tools::BuildLocation(unit, builder, pos);

		case BWAPI::UnitTypes::Protoss_Citadel_of_Adun:
			return Tools::BuildLocation(unit, builder, pos);

		case BWAPI::UnitTypes::Protoss_Templar_Archives:
			return Tools::BuildLocation(unit, builder, pos);
		
		case BWAPI::UnitTypes::Protoss_Forge:
			return Tools::BuildLocation(unit, builder, pos);
		
		case BWAPI::UnitTypes::Protoss_Photon_Cannon:
			return Tools::BuildLocation(unit, builder, pos);
		
		default:
			return false;
	}
}


bool BaseManager::manageSupply(BWAPI::Unit builder)
{
	int currentSupply = Tools::GetTotalSupply(true);

	//Max supply
	if (currentSupply == 200) { return false; }

	//Build supply when available less than 6
	if (BWAPI::Broodwar->self()->supplyUsed() + 6 >= currentSupply)
	{
		return Tools::BuildBuilding(BWAPI::UnitTypes::Protoss_Pylon, builder);
	}
	return false;
}
