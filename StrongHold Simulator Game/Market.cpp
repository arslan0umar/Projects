#include "Stronghold.h"
#include <iostream>
#include <fstream>
using namespace std;

Market::Market() : tradeCount(0) {
    for (int i = 0; i < 50; i++) {
        trades[i].player1 = -1;
        trades[i].player2 = -1;
        trades[i].resource = "";
        trades[i].amount = 0;
        trades[i].isSmuggling = false;
    }
}

void Market::trade(ResourceManager& rm1, ResourceManager& rm2, int resourceNum1, int resourceNum2, int amount, int player1, int player2) {
    try {
        if (amount <= 0) {
            throw GameException("Trade amount must be positive!");
        }
        if (tradeCount >= 50) {
            throw GameException("Trade log is full!");
        }
        string resource1 = rm1.getResourceName(resourceNum1 - 1);
        string resource2 = rm2.getResourceName(resourceNum2 - 1);
        if (!rm1.hasResource(resource1, amount)) {
            throw GameException("Player " + to_string(player1) + " lacks sufficient " + resource1 + "!");
        }
        if (!rm2.hasResource(resource2, amount)) {
            throw GameException("Player " + to_string(player2) + " lacks sufficient " + resource2 + "!");
        }
        rm1.consume(resource1, amount, player1);
        rm2.gather(resource1, amount, player2);
        rm2.consume(resource2, amount, player2);
        rm1.gather(resource2, amount, player1);
        trades[tradeCount].player1 = player1;
        trades[tradeCount].player2 = player2;
        trades[tradeCount].resource = resource1 + " for " + resource2;
        trades[tradeCount].amount = amount;
        trades[tradeCount].isSmuggling = false;
        tradeCount++;
        cout << "Trade successful: Player " << player1 << " traded " << amount << " " << resource1
            << " for " << amount << " " << resource2 << " with Player " << player2 << ".\n";
    }
    catch (const GameException& e) {
        cout << "Error: " << e.message << "\n";
    }
}

void Market::smuggle(ResourceManager& rm1, ResourceManager& rm2, const string& resource, int amount, int player1, int player2) {
    try {
        if (amount <= 0) {
            throw GameException("Smuggle amount must be positive!");
        }
        if (tradeCount >= 50) {
            throw GameException("Trade log is full!");
        }
        if (!rm1.hasResource(resource, amount)) {
            throw GameException("Player " + to_string(player1) + " lacks sufficient " + resource + "!");
        }
        int risk = rand() % 100;
        if (risk < 20) {
            cout << "Smuggling attempt failed! Resources confiscated.\n";
            rm1.consume(resource, amount, player1);
        }
        else {
            rm1.consume(resource, amount, player1);
            rm2.gather(resource, amount, player2);
            cout << "Smuggling successful: " << amount << " " << resource << " transferred from Player "
                << player1 << " to Player " << player2 << ".\n";
        }
        trades[tradeCount].player1 = player1;
        trades[tradeCount].player2 = player2;
        trades[tradeCount].resource = resource;
        trades[tradeCount].amount = amount;
        trades[tradeCount].isSmuggling = true;
        tradeCount++;
    }
    catch (const GameException& e) {
        cout << "Error: " << e.message << "\n";
    }
}

void Market::displayMarket() const {
    cout << "\n===== Market Transactions =====\n";
    bool hasTrades = false;
    for (int i = 0; i < tradeCount; i++) {
        cout << (trades[i].isSmuggling ? "Smuggle" : "Trade") << ": Player " << trades[i].player1
            << " and Player " << trades[i].player2 << ", " << trades[i].amount << " "
            << trades[i].resource << "\n";
        hasTrades = true;
    }
    if (!hasTrades) {
        cout << "No transactions recorded.\n";
    }
}

void Market::saveMarket() const {
    try {
        ofstream file("market.txt");
        if (!file.is_open()) {
            throw GameException("Unable to open market.txt!");
        }
        file << tradeCount << "\n";
        for (int i = 0; i < tradeCount; i++) {
            file << trades[i].player1 << " " << trades[i].player2 << " " << trades[i].resource
                << " " << trades[i].amount << " " << (trades[i].isSmuggling ? 1 : 0) << "\n";
        }
        file.close();
    }
    catch (const GameException& e) {
        cout << "Error: " << e.message << "\n";
    }
}

void Market::loadMarket() {
    try {
        ifstream file("market.txt");
        if (!file.is_open()) {
            throw GameException("Unable to open market.txt!");
        }
        file >> tradeCount;
        for (int i = 0; i < tradeCount; i++) {
            int isSmuggling;
            file >> trades[i].player1 >> trades[i].player2;
            file.ignore();
            getline(file, trades[i].resource, ' ');
            file >> trades[i].amount >> isSmuggling;
            trades[i].isSmuggling = (isSmuggling == 1);
        }
        file.close();
    }
    catch (const GameException& e) {
        cout << "Error: " << e.message << "\n";
    }
}