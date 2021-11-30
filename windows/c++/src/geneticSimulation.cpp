#include "geneticSimulation.h"

// Beginnings of simulation for a GA to use to evaluate fitness
// Abandoned on your advice due to time constraints

geneticSimulation::geneticSimulation()
{
	units[BWAPI::UnitTypes::Protoss_Probe] = 4;
	units[BWAPI::UnitTypes::Protoss_Gateway] = 0;
	units[BWAPI::UnitTypes::Protoss_Zealot] = 0;
	units[BWAPI::UnitTypes::Protoss_Pylon] = 0;
}

// Minerals per min / seconds / frames * workers
int geneticSimulation::calculateIncome()
{
	return 68.1 / 60 / 30 * getUnitCount(BWAPI::UnitTypes::Protoss_Probe);
}

bool geneticSimulation::resourcesAvailable(BWAPI::UnitType unit)
{
	return availableMinerals >= unit.mineralPrice();
}

bool geneticSimulation::supplyAvailable(BWAPI::UnitType unit)
{
	return availableSupply >= unit.supplyRequired();
}

bool geneticSimulation::builderAvailable(BWAPI::UnitType unit)
{
	return units[unit.whatBuilds().first] > 0;
}

bool geneticSimulation::build(BWAPI::UnitType unit)
{
	availableMinerals -= unit.mineralPrice();
	availableSupply -= unit.supplyRequired();

	// Set gateway to busy
	if (unit.whatBuilds().first == BWAPI::UnitTypes::Protoss_Gateway)
	{
		units[BWAPI::UnitTypes::Protoss_Gateway]--;
		unitsBuilding.push_back(std::tuple(BWAPI::UnitTypes::Protoss_Gateway, currentFrame + unit.buildTime()));
	}
}

int geneticSimulation::getUnitCount(BWAPI::UnitType unit)
{
	return units[unit];
}

void geneticSimulation::built()
{
	int i = 0;
	for (auto& unit : unitsBuilding)
	{
		if (std::get<1>(unit) == currentFrame)
		{
			unitsBuilding.erase(unitsBuilding.begin()+i);
			units[std::get<0>(unit)]++;
		}
		i++;
	}
}

BWAPI::UnitType geneticSimulation::getRandomAvailableAction()
{
	// TODO
	//		pick random action from available actions

	return NULL;
}

void geneticSimulation::simulate()
{
	while (units[goal] < goalCount)
	{
		availableMinerals += calculateIncome();

		// add all newly available units to the units map and remove them from the bulding queue
		built();
		
		auto action = getRandomAvailableAction();
		if (action) buildOrder.push_back(std::tuple(action, currentFrame));

		currentFrame++;
	}
}