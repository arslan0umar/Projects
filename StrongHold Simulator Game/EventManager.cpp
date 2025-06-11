#include "Stronghold.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

void EventManager::trigger(ResourceManager& rm, Population& pop, Army& army,
    Peasant& peasant, Merchant& merchant, Noble& noble, int playerId) {
    cout << "\n===== Random Event Triggered =====\n";
    int randEvent = rand() % 5;
    switch (randEvent) {
    case 0: {
        cout << "FAMINE: A devastating famine has struck your kingdom!\n";
        int foodLoss = 30 + (rand() % 20);
        if (rm.hasResource("Food", foodLoss)) {
            rm.consume("Food", foodLoss, playerId);
        }
        else {
            rm.consume("Food", rm.getResourceValue(rm.findResourceIndex("Food")), playerId);
        }
        pop.modify(-(10 + (rand() % 15)));
        army.setMorale(army.getMorale() - 10);
        peasant.updateSatisfaction(-15);
        merchant.updateSatisfaction(-5);
        noble.updateSatisfaction(-3);
        cout << "The famine has severely impacted your kingdom.\n";
        break;
    }
    case 1: {
        cout << "WAR: A neighboring kingdom has declared war!\n";
        int soldierLoss = army.getSoldiers() / 4;
        army.setSoldiers(army.getSoldiers() - soldierLoss);
        army.setMorale(army.getMorale() - 15);
        cout << "You lost " << soldierLoss << " soldiers in battle.\n";
        cout << "Army morale has decreased to " << army.getMorale() << ".\n";
        rm.consume("Gold", 20, playerId);
        rm.consume("Iron", 15, playerId);
        peasant.updateSatisfaction(-10);
        merchant.updateSatisfaction(-8);
        noble.updateSatisfaction(5);
        break;
    }
    case 2: {
        cout << "PLAGUE: A deadly plague spreads through your kingdom!\n";
        int popLoss = pop.getTotal() / 5;
        pop.modify(-popLoss);
        cout << "You lost " << popLoss << " people to the disease.\n";
        rm.consume("Gold", 15, playerId);
        rm.consume("Food", 10, playerId);
        peasant.updateSatisfaction(-12);
        merchant.updateSatisfaction(-10);
        noble.updateSatisfaction(-8);
        break;
    }
    case 3: {
        cout << "GOOD HARVEST: This year's harvest was exceptional!\n";
        int foodGain = 50 + (rand() % 30);
        rm.gather("Food", foodGain, playerId);
        cout << "Food stores increased by " << foodGain << ".\n";
        army.setMorale(army.getMorale() + 5);
        peasant.updateSatisfaction(10);
        merchant.updateSatisfaction(5);
        noble.updateSatisfaction(3);
        break;
    }
    case 4: {
        cout << "TRADE BOOM: A sudden increase in trade has boosted your economy!\n";
        int goldGain = 50 + (rand() % 50);
        rm.gather("Gold", goldGain, playerId);
        cout << "Treasury increased by " << goldGain << " gold.\n";
        peasant.updateSatisfaction(5);
        merchant.updateSatisfaction(15);
        noble.updateSatisfaction(8);
        break;
    }
    }
    cout << "Event effects have been applied to your kingdom.\n";
}