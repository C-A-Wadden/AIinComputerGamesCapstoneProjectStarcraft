#include "InfoManager.h"
#include "Tools.h"

InfoManager::InfoManager()
{

}


void InfoManager::scout()
{

	// If no scout has been assigned select one
	if (!m_scout)
	{
		m_scout = Tools::GetUnitOfType(BWAPI::Broodwar->self()->getRace().getWorker());
	}

	// Explore all start locations
	for (auto& location : BWAPI::Broodwar->getStartLocations())
	{
		//std::cout << location.x << " " << location.y << " Explored: " << BWAPI::Broodwar->isExplored(location) << std::endl;
		if (BWAPI::Broodwar->isExplored(location)) { continue; }
		m_scout->move(BWAPI::Position(location));
	}
}

void InfoManager::setEnemyPos()
{
	if (enemyFound) { return; }
	enemyPos = m_scout->getOrderTargetPosition();
}