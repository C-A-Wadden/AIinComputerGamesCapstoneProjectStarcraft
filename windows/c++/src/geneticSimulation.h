#pragma once

#include <BWAPI.h>
#include "Tools.h"

class geneticSimulation
{
public:

	geneticSimulation();

	int geneticSimulation::calculateIncome();
	bool geneticSimulation::supplyAvailable(BWAPI::UnitType unit);
	bool geneticSimulation::builderAvailable(BWAPI::UnitType unit);
	bool geneticSimulation::resourcesAvailable(BWAPI::UnitType unit);
	bool geneticSimulation::build(BWAPI::UnitType unit);
	int geneticSimulation::getUnitCount(BWAPI::UnitType unit);
	void geneticSimulation::built();
	BWAPI::UnitType geneticSimulation::getRandomAvailableAction();
	void geneticSimulation::simulate();

	int availableMinerals = 50;
	int availableSupply = 6;

	int currentFrame = 1;

	// Goal can be passed in during simulation init
	BWAPI::UnitType goal = BWAPI::UnitTypes::Protoss_Zealot;
	int goalCount = 4;

	//Unit type and the count of completed units
	std::map<BWAPI::UnitType, int> units;

	// Unit and frame it completes, when currentFrame == frameComplete remove from vector and increment map
	std::vector<std::tuple<BWAPI::UnitType, int>> unitsBuilding;

	// tuple(BWAPI::UnitType Action, int frameStarted)
	std::vector<std::tuple<BWAPI::UnitType, int>> buildOrder;

	std::vector<BWAPI::UnitType> possibleActions = { BWAPI::UnitTypes::Protoss_Probe, BWAPI::UnitTypes::Protoss_Gateway,
													BWAPI::UnitTypes::Protoss_Zealot, BWAPI::UnitTypes::Protoss_Pylon };

};