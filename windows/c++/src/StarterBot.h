#pragma once

#include "MapTools.h"

#include <BWAPI.h>
#include "InfoManager.h"
#include "BaseManager.h"
#include "BuildOrder.h"

class StarterBot
{
    MapTools m_mapTools;

public:

    StarterBot();

	InfoManager m_infoManager;
	BaseManager m_baseManager;
	BuildOrder m_buildOrder;

	BWAPI::Unit m_builder = nullptr;

	//are we currently attacking, used for continuous attack after zealots have been pooled
	bool attacking = false;

	//amount of resources spent so far
	int spentMinerals = 0;
	int spentGas = 0;

	int armySize = 0;
	int attackLimit = 4;
	int cleanUpLimit = 11;

	//frames to countdown from the location the enemy was found to where the proxy gates will be built
	int countToProxyPos = 170;
	int countdownToRamp = 200;
	BWAPI::TilePosition buildPos;

	//vector of all gateways created by the player
	std::vector<BWAPI::Unit> gateways = {};

	//vector of all attacking units created by the player
	std::vector<BWAPI::Unit> army = {};

	//vector of enemy buildings encountered
	std::map<int, BWAPI::Unit> enemyBuildings;

    // helper functions to get you started with bot programming and learn the API
    void sendIdleWorkersToMinerals();
    void drawDebugInformation();

    // functions that are triggered by various BWAPI events from main.cpp
	void onStart();
	void onFrame();
	void onEnd(bool isWinner);
	void onUnitDestroy(BWAPI::Unit unit);
	void onUnitMorph(BWAPI::Unit unit);
	void onSendText(std::string text);
	void onUnitCreate(BWAPI::Unit unit);
	void onUnitComplete(BWAPI::Unit unit);
	void onUnitShow(BWAPI::Unit unit);
	void onUnitHide(BWAPI::Unit unit);
	void onUnitRenegade(BWAPI::Unit unit);
};