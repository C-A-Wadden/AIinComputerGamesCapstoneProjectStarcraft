#pragma once

#include <BWAPI.h>
#include "Tools.h"

class BuildOrder
{
public:

	BuildOrder();

	void BuildOrder::proxy2Gate();
	void BuildOrder::dtRush();
	void BuildOrder::speedLots();

	BWAPI::UnitType BuildOrder::getNext();
	int BuildOrder::getNextCost();
	int BuildOrder::getGasNextCost();
	int BuildOrder::getTotalMineralCost();
	int BuildOrder::supplyNeeded();

	std::vector<BWAPI::UnitType> buildOrder;
	BWAPI::UnitType pylon = BWAPI::UnitTypes::Protoss_Pylon;
	BWAPI::UnitType probe = BWAPI::UnitTypes::Protoss_Probe;
	BWAPI::UnitType gateway = BWAPI::UnitTypes::Protoss_Gateway;
	BWAPI::UnitType zealot = BWAPI::UnitTypes::Protoss_Zealot;
	BWAPI::UnitType dragoon = BWAPI::UnitTypes::Protoss_Dragoon;
	BWAPI::UnitType dt = BWAPI::UnitTypes::Protoss_Dark_Templar;
	BWAPI::UnitType citadelOfAdun = BWAPI::UnitTypes::Protoss_Citadel_of_Adun;
	BWAPI::UnitType cyberCore = BWAPI::UnitTypes::Protoss_Cybernetics_Core;
	BWAPI::UnitType assimilator = BWAPI::UnitTypes::Protoss_Assimilator;
	BWAPI::UnitType templarArchives = BWAPI::UnitTypes::Protoss_Templar_Archives;
	BWAPI::UnitType forge = BWAPI::UnitTypes::Protoss_Forge;
	BWAPI::UnitType cannon = BWAPI::UnitTypes::Protoss_Photon_Cannon;


	int index = 0;
	bool proxy = false;
};