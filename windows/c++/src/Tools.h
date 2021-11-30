#pragma once

#include <BWAPI.h>

namespace Tools
{
    BWAPI::Unit GetClosestUnitTo(BWAPI::Position p, const BWAPI::Unitset& units);
    BWAPI::Unit GetClosestUnitTo(BWAPI::Unit unit, const BWAPI::Unitset& units);

    int CountUnitsOfType(BWAPI::UnitType type, const BWAPI::Unitset& units);

    BWAPI::Unit GetUnitOfType(BWAPI::UnitType type);
    std::vector<BWAPI::Unit> GetAllUnitsOfType(BWAPI::UnitType type);
    BWAPI::Unit GetDepot();

    bool BuildBuilding(BWAPI::UnitType type, BWAPI::Unit builder);

    bool BuildLocation(BWAPI::UnitType type, BWAPI::Unit builder, BWAPI::TilePosition pos);

    void DrawUnitBoundingBoxes();
    void DrawUnitCommands();

    void SmartRightClick(BWAPI::Unit unit, BWAPI::Unit target);

    int GetTotalSupply(bool inProgress = false);

    void DrawUnitHealthBars();
    void DrawHealthBar(BWAPI::Unit unit, double ratio, BWAPI::Color color, int yOffset);

    bool TrainWorker(bool queue);
    
    bool TrainAtGateway(std::vector<BWAPI::Unit> gateways, BWAPI::UnitType unit, bool queue);
    bool BuildAssimilator(BWAPI::Unit builder, BWAPI::Unit geyser);
}