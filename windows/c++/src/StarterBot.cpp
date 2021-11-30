#include "StarterBot.h"
#include "Tools.h"
#include "MapTools.h"
#include "InfoManager.h"
#include "BaseManager.h"
#include "BuildOrder.h"

StarterBot::StarterBot()
{
    
}

// Called when the bot starts!
void StarterBot::onStart()
{
    // Set our BWAPI options here    
	BWAPI::Broodwar->setLocalSpeed(5);
    BWAPI::Broodwar->setFrameSkip(0);
    
    // Enable the flag that tells BWAPI to let users enter input while bot plays
    BWAPI::Broodwar->enableFlag(BWAPI::Flag::UserInput);

    // Call MapTools OnStart
    m_mapTools.onStart();

    m_infoManager = InfoManager::InfoManager();
    m_baseManager = BaseManager::BaseManager();
    m_buildOrder  = BuildOrder::BuildOrder();
}

// Called whenever the game ends and tells you if you won or not
void StarterBot::onEnd(bool isWinner) 
{
    //std::cout << "We " << (isWinner ? "won!" : "lost!") << "\n";
}

// Called on each frame of the game
void StarterBot::onFrame()
{
    //std::cout << BWAPI::Broodwar->getFrameCount() << std::endl;
    //std::cout << "EnemyPos " << m_infoManager.enemyPos.x << " " << m_infoManager.enemyPos.y << std::endl;
    auto myDepot = Tools::GetDepot();
    
    //if (BWAPI::Broodwar->self()->supplyUsed() > 30)
    //{
    //    bool enoughGas = (BWAPI::Broodwar->self()->gatheredGas() - spentGas) >= 150;
    //    bool enoughMinerals = (BWAPI::Broodwar->self()->gatheredMinerals() - spentMinerals) >= 150;

    //    auto citadel = Tools::GetUnitOfType(m_buildOrder.citadelOfAdun);
    //    if (citadel && enoughGas && enoughMinerals)
    //    {
    //        if (citadel->upgrade(BWAPI::UpgradeTypes::Leg_Enhancements))
    //        {
    //            spentGas += 150; spentMinerals += 150;
    //        }
    //    }
    //}
    
    //if we have no depot give up
    if (!myDepot) { return; }

    //Build at home
    buildPos = myDepot->getTilePosition();

    // Find a usable proxy position
    if (m_infoManager.enemyFound) countToProxyPos--;
    if (m_buildOrder.proxy && countToProxyPos == 0 && m_infoManager.m_scout)
    { 
        m_infoManager.m_scout->stop();
        m_infoManager.proxyPos = m_infoManager.m_scout->getPosition();
    }

    // Build at proxy location
    if (m_buildOrder.proxy && countToProxyPos <= 0 && m_buildOrder.index > 7) buildPos = BWAPI::TilePosition(m_infoManager.proxyPos);

    // Update our MapTools information
    m_mapTools.onFrame();

    const BWAPI::UnitType workerType = BWAPI::Broodwar->self()->getRace().getWorker();
    const BWAPI::Unitset& myUnits = BWAPI::Broodwar->self()->getUnits();
    const int workersOwned = Tools::CountUnitsOfType(workerType, myUnits);

    // Send our idle workers to mine minerals so they don't just stand there
    sendIdleWorkersToMinerals();

    // Get geyser close to first nexus
    BWAPI::Unit geyser = nullptr;
    if (myDepot) { geyser = Tools::GetClosestUnitTo(myDepot, BWAPI::Broodwar->getGeysers()); }

    if (!m_builder) m_builder = Tools::GetUnitOfType(workerType);
    
    //Don't build with scout unless proxying or its all thats left
    int count = 0;
    while (!m_buildOrder.proxy && m_builder == m_infoManager.m_scout && count < 100)
    {
        count++;
        m_builder = Tools::GetUnitOfType(workerType);
    }

    if (workersOwned == 7) m_builder->stop();

    // Execute build order
    if (m_buildOrder.index < m_buildOrder.buildOrder.size())
    {
        auto nextBuilt = m_buildOrder.getNext();

        // Build cannons closer to front of base
        if (nextBuilt == BWAPI::UnitTypes::Protoss_Photon_Cannon) buildPos = BWAPI::TilePosition(m_infoManager.ramp);
        
        //check if we can afford next item in build order
        // TODO add supply
        bool enoughGas = (BWAPI::Broodwar->self()->gatheredGas() - spentGas) >= m_buildOrder.getGasNextCost();
        bool enoughMinerals = (BWAPI::Broodwar->self()->gatheredMinerals() - spentMinerals) >= m_buildOrder.getNextCost();

        if (enoughMinerals && enoughGas)
        {
            //std::cout << nextBuilt.getName() << " Minerals: " << BWAPI::Broodwar->self()->gatheredMinerals() - spentMinerals;
            //std::cout << " Gas: " << BWAPI::Broodwar->self()->gatheredGas() - spentGas << std::endl;
            if (nextBuilt.whatBuilds().first == BWAPI::UnitTypes::Protoss_Nexus)
            {
                if (Tools::TrainWorker(true))
                {
                    spentMinerals += m_buildOrder.getNextCost();
                    spentGas += m_buildOrder.getGasNextCost();
                    m_buildOrder.index += 1;
                }
            }

            else if (nextBuilt.whatBuilds().first == BWAPI::UnitTypes::Protoss_Gateway)
            {
                if (Tools::TrainAtGateway(gateways, nextBuilt, true))
                {
                    spentMinerals += m_buildOrder.getNextCost();
                    spentGas += m_buildOrder.getGasNextCost();
                    m_buildOrder.index += 1;
                }
            }

            else if (nextBuilt.whatBuilds().first == BWAPI::UnitTypes::Protoss_Probe)
            {
                if (nextBuilt == BWAPI::UnitTypes::Protoss_Assimilator)
                {
                    if (m_builder && geyser && Tools::BuildAssimilator(m_builder, geyser))
                    {
                        spentMinerals += m_buildOrder.getNextCost();
                        spentGas += m_buildOrder.getGasNextCost();
                        m_buildOrder.index += 1;
                    }
                }

                //Controls scout movement from enemy location to proxy location
                else if (!m_buildOrder.proxy || countToProxyPos <= 0)
                {
                    // Set scout to builder for proxying buildings
                    if (m_buildOrder.proxy && m_buildOrder.index > 7) m_builder = m_infoManager.m_scout;

                    if (m_builder && m_builder->isIdle() && m_baseManager.build(nextBuilt, m_builder, buildPos, true))
                    {
                        spentMinerals += m_buildOrder.getNextCost();
                        spentGas += m_buildOrder.getGasNextCost();
                        m_buildOrder.index += 1;
                    }
                }
            }
        }
    }
    else // Post build order management
    {
        // Build at home
        m_buildOrder.proxy = false;
        int count = 0;

        // Send the scout back to work if he has found the enemy and finished proxying
        //if (m_infoManager.enemyFound) m_infoManager.m_scout = nullptr;

        // Get a builder probe
        while (!m_builder || m_builder == m_infoManager.m_scout && count < 100)
        {
            count++;
            m_builder = Tools::GetUnitOfType(m_buildOrder.probe);
        }

        // Build pylons as needed
        if (m_builder) 
        { 
            if (m_baseManager.manageSupply(m_builder))
            {
                spentMinerals += BWAPI::UnitTypes::Protoss_Pylon.mineralPrice();
            }
        }

        // Continue producing army
        if (Tools::TrainAtGateway(gateways, BWAPI::UnitTypes::Protoss_Dark_Templar, false))
        {
            spentMinerals += BWAPI::UnitTypes::Protoss_Dark_Templar.mineralPrice();
            spentGas += BWAPI::UnitTypes::Protoss_Dark_Templar.gasPrice();
        }
        if (Tools::TrainAtGateway(gateways, BWAPI::UnitTypes::Protoss_Dragoon, false))
        {
            spentMinerals += BWAPI::UnitTypes::Protoss_Dragoon.mineralPrice();
            spentGas += BWAPI::UnitTypes::Protoss_Dragoon.gasPrice();
        }
        if (Tools::TrainAtGateway(gateways, BWAPI::UnitTypes::Protoss_Zealot, false))
        {
            spentMinerals += BWAPI::UnitTypes::Protoss_Zealot.mineralPrice();
            spentGas += BWAPI::UnitTypes::Protoss_Zealot.gasPrice();
        }
    }

    // Attacking functionality
    
    // Scout the map to find the enemy
    if (workersOwned >= 6 && !m_infoManager.enemyFound)
    {
        m_infoManager.scout();

        // Estimate position of ramp
        countdownToRamp--;
        if (countdownToRamp == 0) { m_infoManager.ramp = m_infoManager.m_scout->getPosition(); }
    }

    //Pool army then attack main base, continue to attack once the attack has started
    if (armySize > attackLimit || (attacking && armySize > 4))
    {
        // TODO modify the x, y of the attackmove coordinates as time goes on ranging further afield
        attacking = true;
        for (auto unit : army)
        {
            if (unit->exists() && unit->isIdle()) unit->attack(m_infoManager.enemyPos);
        }
    }

    //Finish off all buildings
    if (armySize > cleanUpLimit || BWAPI::Broodwar->getFrameCount() > 15000)
    {
        for (auto itr = enemyBuildings.begin(); itr != enemyBuildings.end(); itr++)
        {
            auto building = itr->second;
            if (!building || !building->exists()) { continue; }
            BWAPI::Unit zealot = Tools::GetUnitOfType(BWAPI::UnitTypes::Protoss_Zealot);
            BWAPI::Unit dt = Tools::GetUnitOfType(BWAPI::UnitTypes::Protoss_Dark_Templar);
            BWAPI::Unit dragoon = Tools::GetUnitOfType(BWAPI::UnitTypes::Protoss_Dragoon);
            if (dt && dt->exists() && !dt->isAttacking() && !dt->isMoving())                     Tools::SmartRightClick(dt, (building));
            if (zealot && zealot->exists() && !zealot->isAttacking() && !zealot->isMoving())     Tools::SmartRightClick(zealot, (building));
            if (dragoon && dragoon->exists() && !dragoon->isAttacking() && !dragoon->isMoving()) Tools::SmartRightClick(dragoon, (building));
        }
    }

    // Draw unit health bars, which brood war unfortunately does not do
    Tools::DrawUnitHealthBars();

    // Draw some relevent information to the screen to help us debug the bot
    drawDebugInformation();
}

// Send our idle workers to mine minerals so they don't just stand there
void StarterBot::sendIdleWorkersToMinerals()
{
    const BWAPI::Unitset& myUnits = BWAPI::Broodwar->self()->getUnits();

    auto myDepot = Tools::GetDepot();
    BWAPI::Unit closestMineral = nullptr;
    if (myDepot) { closestMineral = Tools::GetClosestUnitTo(myDepot, BWAPI::Broodwar->getMinerals()); }
    
    for (auto& unit : myUnits)
    {
        // Don't return the scout or builder to gather
        if (unit == m_infoManager.m_scout || unit == m_builder) { continue; }

        // Check the unit type, if it is an idle worker, then we want to send it somewhere
        if (unit->getType().isWorker() && unit->isIdle())
        {
            // If a valid mineral was found, right click it with the unit in order to start harvesting
            if (closestMineral) { unit->rightClick(closestMineral); }
        }
    }
}

// Draw some relevent information to the screen to help us debug the bot
void StarterBot::drawDebugInformation()
{
    BWAPI::Broodwar->drawTextScreen(BWAPI::Position(10, 10), "Charles Wadden  201754280\n");
    BWAPI::Broodwar->drawTextScreen(BWAPI::Position(10, 20), "Jordan Noel     201710845\n");
    Tools::DrawUnitCommands();
    Tools::DrawUnitBoundingBoxes();
}

// Called whenever a unit is destroyed, with a pointer to the unit
void StarterBot::onUnitDestroy(BWAPI::Unit unit)
{
    // If the scout is killed reassign
    if (unit == m_infoManager.m_scout)
    {
        m_infoManager.m_scout = Tools::GetUnitOfType(m_buildOrder.probe);
        int count = 0;
        while (m_builder == m_infoManager.m_scout && count < 100)
        {
            count++;
            m_infoManager.m_scout = Tools::GetUnitOfType(m_buildOrder.probe);
        }
    }

    // If the builder is killed reassign
    if (unit == m_builder)
    {
        m_builder = Tools::GetUnitOfType(m_buildOrder.probe);
        int count = 0;
        while (m_builder == m_infoManager.m_scout && count < 100)
        {
            count++;
            m_builder = Tools::GetUnitOfType(m_buildOrder.probe);
        }
    }

    // Keep track of new army size as units die
    if (unit->getType() == BWAPI::UnitTypes::Protoss_Zealot || unit->getType() == BWAPI::UnitTypes::Protoss_Dark_Templar || unit->getType() == BWAPI::UnitTypes::Protoss_Dragoon)
    {
        armySize--;
    }
}

// Called whenever a unit is morphed, with a pointer to the unit
// Zerg units morph when they turn into other units
void StarterBot::onUnitMorph(BWAPI::Unit unit)
{
	
}

// Called whenever a text is sent to the game by a user
void StarterBot::onSendText(std::string text) 
{ 
    if (text == "/map")
    {
        m_mapTools.toggleDraw();
    }
}

// Called whenever a unit is created, with a pointer to the unit
// Units are created in buildings like barracks before they are visible, 
// so this will trigger when you issue the build command for most units
void StarterBot::onUnitCreate(BWAPI::Unit unit)
{ 
    
}

// Called whenever a unit finished construction, with a pointer to the unit
void StarterBot::onUnitComplete(BWAPI::Unit unit)
{
    if (unit->getType() == BWAPI::UnitTypes::Protoss_Assimilator)
    {
        // Send 3 workers to gather
        int count = 0;
        while (count < 3)
        {
            auto probe = Tools::GetUnitOfType(BWAPI::UnitTypes::Protoss_Probe);
            if (probe != m_builder && probe != m_infoManager.m_scout)
            {
                probe->rightClick(unit);
                count++;
            }
        }
    }

    // Store army and gateways for convient lookup
    if (unit->getType() == BWAPI::UnitTypes::Protoss_Gateway)
    {
        gateways.push_back(unit);
    }
    else if ((unit->getType() == BWAPI::UnitTypes::Protoss_Zealot) || (unit->getType() == BWAPI::UnitTypes::Protoss_Dark_Templar) || (unit->getType() == BWAPI::UnitTypes::Protoss_Dragoon))
    {
        if (!m_buildOrder.proxy) unit->attack(m_infoManager.ramp);
        else unit->attack(m_infoManager.proxyPos);
        army.push_back(unit);
        armySize++;
    }
}

// Called whenever a unit appears, with a pointer to the unit
// This is usually triggered when units appear from fog of war and become visible
void StarterBot::onUnitShow(BWAPI::Unit unit)
{ 
    //if statement that records the enemy position when the scout encounters the enemy for the first time
    if (unit->getPlayer() == BWAPI::Broodwar->enemy() && !m_infoManager.enemyFound)
    {
        m_infoManager.setEnemyPos();
        m_infoManager.enemyFound = true;

        // Modifiy build order based on race encountered
        if (unit->getType().getRace() == BWAPI::Races::Zerg)
        {
            m_buildOrder.proxy2Gate();
            m_buildOrder.proxy = true;
            attackLimit = 5;
            countToProxyPos += 20;
            auto myDepot = Tools::GetDepot();
            if (myDepot && m_infoManager.m_scout) { m_infoManager.m_scout->rightClick(myDepot); }
        }
        else if (unit->getType().getRace() == BWAPI::Races::Protoss)
        {
            m_buildOrder.dtRush();
            auto myDepot = Tools::GetDepot();
            attackLimit = 11;
            cleanUpLimit = 20;
            if (myDepot && m_infoManager.m_scout) { m_infoManager.m_scout = nullptr; }
        }
        else
        {
            m_buildOrder.proxy2Gate();
            m_buildOrder.proxy = true;
            auto myDepot = Tools::GetDepot();
            if (myDepot && m_infoManager.m_scout) { m_infoManager.m_scout->rightClick(myDepot); }
        }
    }
    
    // Keep a vector of building seen to finish off after battle is won
    if (unit->getPlayer() == BWAPI::Broodwar->enemy() && !unit->canAttack())
    {
        enemyBuildings[unit->getID()] = unit;
    }
}

// Called whenever a unit gets hidden, with a pointer to the destroyed unit
// This is usually triggered when units enter the fog of war and are no longer visible
void StarterBot::onUnitHide(BWAPI::Unit unit)
{ 
	
}

// Called whenever a unit switches player control
// This usually happens when a dark archon takes control of a unit
void StarterBot::onUnitRenegade(BWAPI::Unit unit)
{ 
	
}