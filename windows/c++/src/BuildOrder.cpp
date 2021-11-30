#include "BuildOrder.h"
#include "Tools.h"


BuildOrder::BuildOrder()
{
	// Set openning build order
	buildOrder = { probe, probe, probe, probe, pylon, probe, gateway, probe };
}

// 9/9 gate vs terran zealot rush
void BuildOrder::proxy2Gate()
{
	std::vector<BWAPI::UnitType> buildOrder2 = { pylon, probe, zealot, gateway, zealot, zealot, zealot, gateway, zealot, zealot, zealot };
	buildOrder.insert(buildOrder.end(), buildOrder2.begin(), buildOrder2.end());
}

// Four gate dt timing attack build made using BOSS website and some hand tweaking
// Intended for use against Protoss
void BuildOrder::dtRush()
{
	std::vector<BWAPI::UnitType> buildOrder2 = {
		    probe, probe, assimilator,
			probe, zealot, probe, pylon, probe, probe, cyberCore, probe, zealot, probe, pylon, probe, zealot,
			probe, citadelOfAdun, probe, dragoon, gateway, pylon, templarArchives, forge, zealot, zealot,
			pylon, dt, gateway, cannon, dt, probe, probe, gateway, cannon
	};
	buildOrder.insert(buildOrder.end(), buildOrder2.begin(), buildOrder2.end());
}

// Speedlot timing attack intended for use vs zerg
// Performance not as good as originally hoped
void BuildOrder::speedLots()
{
	std::vector<BWAPI::UnitType> buildOrder2 = {
			probe, probe, assimilator,
			probe, zealot, probe, pylon, probe, zealot, probe, cyberCore, probe, zealot, pylon, probe, probe,
			dragoon, citadelOfAdun, probe, probe, gateway, zealot, pylon, probe, probe, zealot, zealot,
			pylon, gateway, templarArchives, zealot, zealot, dt, dt, gateway
	};
	buildOrder.insert(buildOrder.end(), buildOrder2.begin(), buildOrder2.end());
}

BWAPI::UnitType BuildOrder::getNext()
{
	return buildOrder[index];
}

int BuildOrder::getNextCost()
{
	return buildOrder[index].mineralPrice();
}

int BuildOrder::getGasNextCost()
{
	return buildOrder[index].gasPrice();
}

int BuildOrder::getTotalMineralCost()
{
	int cost = 0;
	for (auto& unit : buildOrder)
	{
		cost += unit.mineralPrice();
	}
	return cost;
}

int BuildOrder::supplyNeeded()
{
	int supply = 0;
	for (auto& unit : buildOrder)
	{
		supply += unit.supplyRequired();
	}
	return ceil(supply/8);
}
