#include "Stronghold.h"
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

void Logger::logScore(const string& info) {
    try {
        ofstream file("score_log.txt", ios::app);
        if (!file.is_open()) {
            throw GameException("Unable to open score_log.txt!");
        }
        file << info << "\n";
        file.close();
    }
    catch (const GameException& e) {
        cout << "Error: " << e.message << "\n";
    }
}

void Logger::saveGame(const Population& pop, const Army& army,
    const Bank& bank, const Leader& leader,
    const ResourceManager& rm, const Economy& economy,
    const Peasant& peasant, const Merchant& merchant, const Noble& noble) {
    try {
        ofstream file("game_save.txt");
        if (!file.is_open()) {
            throw GameException("Unable to open game_save.txt for saving!");
        }
        file << "POPULATION " << pop.getTotal() << "\n";
        file << "ARMY " << army.getSoldiers() << " " << army.getMorale() << "\n";
        file << "BANK " << bank.getLoan() << "\n";
        file << "LEADER " << leader.getName() << " " << leader.getPopularity() << "\n";
        file << "ECONOMY " << economy.getTaxRate() << "\n";
        file << "SOCIAL_CLASSES " << peasant.getSatisfaction() << " "
            << merchant.getSatisfaction() << " " << noble.getSatisfaction() << "\n";
        file << "RESOURCES " << rm.getResourceCount() << "\n";
        for (int i = 0; i < rm.getResourceCount(); i++) {
            file << rm.getResourceName(i) << " " << rm.getResourceValue(i) << "\n";
        }
        file.close();
        cout << "Game saved successfully.\n";
    }
    catch (const GameException& e) {
        cout << "Error: " << e.message << "\n";
    }
}

bool Logger::loadGame(Population& pop, Army& army, Bank& bank,
    Leader& leader, ResourceManager& rm, Economy& economy,
    Peasant& peasant, Merchant& merchant, Noble& noble) {
    try {
        ifstream file("game_save.txt");
        if (!file.is_open()) {
            throw GameException("Unable to open game_save.txt for loading!");
        }
        string marker;
        // Load Population
        int totalPop;
        file >> marker >> totalPop;
        pop.setTotal(totalPop);
        // Load Army
        int soldiers, morale;
        file >> marker >> soldiers >> morale;
        army.setSoldiers(soldiers);
        army.setMorale(morale);
        // Load Bank
        int loan;
        file >> marker >> loan;
        bank.setLoan(loan);
        // Load Leader
        file >> marker;
        string name;
        getline(file >> ws, name, ' ');
        int popularity;
        file >> popularity;
        leader.setName(name);
        leader.setPopularity(popularity);
        // Load Economy
        int taxRate;
        file >> marker >> taxRate;
        economy.setTaxRate(taxRate);
        // Load Social Classes
        int peasantSat, merchantSat, nobleSat;
        file >> marker >> peasantSat >> merchantSat >> nobleSat;
        peasant.updateSatisfaction(peasantSat - peasant.getSatisfaction());
        merchant.updateSatisfaction((merchantSat - merchant.getSatisfaction()) / 2);
        noble.updateSatisfaction((nobleSat - noble.getSatisfaction()) * 2);
        // Load Resources
        int resourceCount;
        file >> marker >> resourceCount;
        ResourceManager tempRM;
        for (int i = 0; i < resourceCount; i++) {
            string resourceName;
            int amount;
            file >> resourceName >> amount;
            tempRM.gather(resourceName, amount, 0);
        }
        rm = tempRM;
        file.close();
        cout << "Game loaded successfully.\n";
        return true;
    }
    catch (const GameException& e) {
        cout << "Error: " << e.message << "\n";
        return false;
    }
}