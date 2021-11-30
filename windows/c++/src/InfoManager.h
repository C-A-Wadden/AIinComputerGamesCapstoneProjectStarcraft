#pragma once

#include <BWAPI.h>

class InfoManager
{
public:
	InfoManager();
	BWAPI::Unit m_scout = nullptr;

	bool enemyFound = false;
	BWAPI::Position enemyPos;
	BWAPI::Position proxyPos;
	BWAPI::Position ramp;

	void InfoManager::scout();
	void InfoManager::setEnemyPos();
};