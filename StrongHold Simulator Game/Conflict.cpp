#include "Stronghold.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <iomanip>
using namespace std;

Conflict::Conflict() : warCount(0) {
    for (int i = 0; i < 20; i++) {
        wars[i].attacker = -1;
        wars[i].defender = -1;
        wars[i].active = false;
    }
}

void Conflict::declareWar(int attacker, int defender, Army& attackerArmy, Army& defenderArmy,
    Leader& attackerLeader, Leader& defenderLeader,
    Peasant& attackerPeasant, Merchant& attackerMerchant, Noble& attackerNoble,
    Peasant& defenderPeasant, Merchant& defenderMerchant, Noble& defenderNoble) {
    try {
        if (attacker == defender) {
            throw GameException("Cannot declare war on yourself!");
        }
        if (attackerArmy.getSoldiers() <= 0 || defenderArmy.getSoldiers() <= 0) {
            throw GameException("One or both kingdoms have no soldiers to fight!");
        }
        for (int i = 0; i < warCount; i++) {
            if (wars[i].attacker == attacker && wars[i].defender == defender && wars[i].active) {
                throw GameException("War already active between Player " + to_string(attacker) +
                    " and Player " + to_string(defender) + "!");
            }
        }
        if (warCount >= 20) {
            throw GameException("War log is full!");
        }

        cout << "\n====================================\n";
        cout << "WAR DECLARED: Player " << attacker << " vs Player " << defender << "\n";
        cout << "====================================\n";

        // Calculate strengths
        int attackerStrength = attackerArmy.getSoldiers() * (attackerArmy.getMorale() / 20);
        int defenderStrength = defenderArmy.getSoldiers() * (defenderArmy.getMorale() / 20);

        // Store initial values for display
        int attackerInitialSoldiers = attackerArmy.getSoldiers();
        int defenderInitialSoldiers = defenderArmy.getSoldiers();
        int attackerInitialMorale = attackerArmy.getMorale();
        int defenderInitialMorale = defenderArmy.getMorale();
        int attackerInitialPopularity = attackerLeader.getPopularity();
        int defenderInitialPopularity = defenderLeader.getPopularity();
        int attackerPeasantSatisfaction = attackerPeasant.getSatisfaction();
        int attackerMerchantSatisfaction = attackerMerchant.getSatisfaction();
        int attackerNobleSatisfaction = attackerNoble.getSatisfaction();
        int defenderPeasantSatisfaction = defenderPeasant.getSatisfaction();
        int defenderMerchantSatisfaction = defenderMerchant.getSatisfaction();
        int defenderNobleSatisfaction = defenderNoble.getSatisfaction();

        if (attackerStrength > defenderStrength) {
            // Attacker wins
            int soldierLoss = defenderInitialSoldiers / 4;
            int attackerLoss = soldierLoss / 2;
            attackerArmy.setSoldiers(attackerInitialSoldiers - attackerLoss);
            defenderArmy.setSoldiers(defenderInitialSoldiers - soldierLoss);
            attackerArmy.setMorale(attackerInitialMorale + 10);
            defenderArmy.setMorale(defenderInitialMorale - 15);
            attackerLeader.setPopularity(attackerInitialPopularity + 5);
            defenderLeader.setPopularity(defenderInitialPopularity - 10);
            attackerPeasant.updateSatisfaction(2);
            attackerMerchant.updateSatisfaction(4);
            attackerNoble.updateSatisfaction(1);
            defenderPeasant.updateSatisfaction(-5);
            defenderMerchant.updateSatisfaction(-10);
            defenderNoble.updateSatisfaction(-2);

            cout << "Outcome: Player " << attacker << " defeats Player " << defender << "!\n";
            cout << "\n--- Player " << attacker << " (Winner) ---\n";
            cout << "Soldiers: " << attackerInitialSoldiers << " -> " << attackerArmy.getSoldiers()
                << " (Lost: " << attackerLoss << ")\n";
            cout << "Morale: " << attackerInitialMorale << " -> " << attackerArmy.getMorale()
                << " (+" << 10 << ")\n";
            cout << "Leader Popularity: " << attackerInitialPopularity << " -> "
                << attackerLeader.getPopularity() << " (+" << 5 << ")\n";
            cout << "Peasant Satisfaction: " << attackerPeasantSatisfaction << " -> "
                << attackerPeasant.getSatisfaction() << " (+" << 2 << ")\n";
            cout << "Merchant Satisfaction: " << attackerMerchantSatisfaction << " -> "
                << attackerMerchant.getSatisfaction() << " (+" << 4 << ")\n";
            cout << "Noble Satisfaction: " << attackerNobleSatisfaction << " -> "
                << attackerNoble.getSatisfaction() << " (+" << 1 << ")\n";
            cout << "\n--- Player " << defender << " (Loser) ---\n";
            cout << "Soldiers: " << defenderInitialSoldiers << " -> " << defenderArmy.getSoldiers()
                << " (Lost: " << soldierLoss << ")\n";
            cout << "Morale: " << defenderInitialMorale << " -> " << defenderArmy.getMorale()
                << " (-" << 15 << ")\n";
            cout << "Leader Popularity: " << defenderInitialPopularity << " -> "
                << defenderLeader.getPopularity() << " (-" << 10 << ")\n";
            cout << "Peasant Satisfaction: " << defenderPeasantSatisfaction << " -> "
                << defenderPeasant.getSatisfaction() << " (-" << 5 << ")\n";
            cout << "Merchant Satisfaction: " << defenderMerchantSatisfaction << " -> "
                << defenderMerchant.getSatisfaction() << " (-" << 10 << ")\n";
            cout << "Noble Satisfaction: " << defenderNobleSatisfaction << " -> "
                << defenderNoble.getSatisfaction() << " (-" << 2 << ")\n";
        }
        else {
            // Defender wins
            int soldierLoss = attackerInitialSoldiers / 4;
            int defenderLoss = soldierLoss / 2;
            attackerArmy.setSoldiers(attackerInitialSoldiers - soldierLoss);
            defenderArmy.setSoldiers(defenderInitialSoldiers - defenderLoss);
            attackerArmy.setMorale(attackerInitialMorale - 15);
            defenderArmy.setMorale(defenderInitialMorale + 10);
            attackerLeader.setPopularity(attackerInitialPopularity - 10);
            defenderLeader.setPopularity(defenderInitialPopularity + 5);
            attackerPeasant.updateSatisfaction(-5);
            attackerMerchant.updateSatisfaction(-10);
            attackerNoble.updateSatisfaction(-2);
            defenderPeasant.updateSatisfaction(2);
            defenderMerchant.updateSatisfaction(4);
            defenderNoble.updateSatisfaction(1);

            cout << "Outcome: Player " << defender << " repels Player " << attacker << "'s attack!\n";
            cout << "\n--- Player " << defender << " (Winner) ---\n";
            cout << "Soldiers: " << defenderInitialSoldiers << " -> " << defenderArmy.getSoldiers()
                << " (Lost: " << defenderLoss << ")\n";
            cout << "Morale: " << defenderInitialMorale << " -> " << defenderArmy.getMorale()
                << " (+" << 10 << ")\n";
            cout << "Leader Popularity: " << defenderInitialPopularity << " -> "
                << defenderLeader.getPopularity() << " (+" << 5 << ")\n";
            cout << "Peasant Satisfaction: " << defenderPeasantSatisfaction << " -> "
                << defenderPeasant.getSatisfaction() << " (+" << 2 << ")\n";
            cout << "Merchant Satisfaction: " << defenderMerchantSatisfaction << " -> "
                << defenderMerchant.getSatisfaction() << " (+" << 4 << ")\n";
            cout << "Noble Satisfaction: " << defenderNobleSatisfaction << " -> "
                << defenderNoble.getSatisfaction() << " (+" << 1 << ")\n";
            cout << "\n--- Player " << attacker << " (Loser) ---\n";
            cout << "Soldiers: " << attackerInitialSoldiers << " -> " << attackerArmy.getSoldiers()
                << " (Lost: " << soldierLoss << ")\n";
            cout << "Morale: " << attackerInitialMorale << " -> " << attackerArmy.getMorale()
                << " (-" << 15 << ")\n";
            cout << "Leader Popularity: " << attackerInitialPopularity << " -> "
                << attackerLeader.getPopularity() << " (-" << 10 << ")\n";
            cout << "Peasant Satisfaction: " << attackerPeasantSatisfaction << " -> "
                << attackerPeasant.getSatisfaction() << " (-" << 5 << ")\n";
            cout << "Merchant Satisfaction: " << attackerMerchantSatisfaction << " -> "
                << attackerMerchant.getSatisfaction() << " (-" << 10 << ")\n";
            cout << "Noble Satisfaction: " << attackerNobleSatisfaction << " -> "
                << attackerNoble.getSatisfaction() << " (-" << 2 << ")\n";
        }
        cout << "====================================\n";

        wars[warCount].attacker = attacker;
        wars[warCount].defender = defender;
        wars[warCount].active = true;
        warCount++;
    }
    catch (const GameException& e) {
        cout << "Error: " << e.message << "\n";
    }
}

void Conflict::betray(int betrayer, int victim, Diplomacy& diplomacy, Army& betrayerArmy, Army& victimArmy,
    Leader& betrayerLeader, Leader& victimLeader,
    Peasant& betrayerPeasant, Merchant& betrayerMerchant, Noble& betrayerNoble,
    Peasant& victimPeasant, Merchant& victimMerchant, Noble& victimNoble) {
    try {
        if (!diplomacy.hasAlliance(betrayer, victim)) {
            throw GameException("No alliance exists to betray between Player " + to_string(betrayer) +
                " and Player " + to_string(victim) + "!");
        }
        if (betrayerArmy.getSoldiers() <= 0 || victimArmy.getSoldiers() <= 0) {
            throw GameException("One or both kingdoms have no soldiers to fight!");
        }
        diplomacy.breakAlliance(betrayer, victim);
        int betrayerStrength = betrayerArmy.getSoldiers() * (betrayerArmy.getMorale() / 20);
        int victimStrength = victimArmy.getSoldiers() * (victimArmy.getMorale() / 20);
        if (betrayerStrength > victimStrength) {
            int soldierLoss = victimArmy.getSoldiers() / 3;
            victimArmy.setSoldiers(victimArmy.getSoldiers() - soldierLoss);
            betrayerArmy.setSoldiers(betrayerArmy.getSoldiers() - soldierLoss / 2);
            betrayerArmy.setMorale(betrayerArmy.getMorale() + 15);
            victimArmy.setMorale(victimArmy.getMorale() - 20);
            betrayerLeader.setPopularity(betrayerLeader.getPopularity() - 15);
            victimLeader.setPopularity(victimLeader.getPopularity() - 10);
            betrayerPeasant.updateSatisfaction(-10);
            betrayerMerchant.updateSatisfaction(-5);
            betrayerNoble.updateSatisfaction(3);
            victimPeasant.updateSatisfaction(-15);
            victimMerchant.updateSatisfaction(-10);
            victimNoble.updateSatisfaction(-8);
            cout << "Player " << betrayer << " betrayed Player " << victim << " and won the battle!\n";
        }
        else {
            int soldierLoss = betrayerArmy.getSoldiers() / 3;
            betrayerArmy.setSoldiers(betrayerArmy.getSoldiers() - soldierLoss);
            victimArmy.setSoldiers(victimArmy.getSoldiers() - soldierLoss / 2);
            betrayerArmy.setMorale(betrayerArmy.getMorale() - 20);
            victimArmy.setMorale(victimArmy.getMorale() + 15);
            betrayerLeader.setPopularity(betrayerLeader.getPopularity() - 20);
            victimLeader.setPopularity(victimLeader.getPopularity() - 5);
            betrayerPeasant.updateSatisfaction(-15);
            betrayerMerchant.updateSatisfaction(-10);
            betrayerNoble.updateSatisfaction(-8);
            victimPeasant.updateSatisfaction(-10);
            victimMerchant.updateSatisfaction(-5);
            victimNoble.updateSatisfaction(3);
            cout << "Player " << victim << " repelled Player " << betrayer << "'s betrayal!\n";
        }
        wars[warCount].attacker = betrayer;
        wars[warCount].defender = victim;
        wars[warCount].active = true;
        warCount++;
    }
    catch (const GameException& e) {
        cout << "Error: " << e.message << "\n";
    }
}

void Conflict::displayWars() const {
    cout << "\n===== Active Wars =====\n";
    bool hasWars = false;
    for (int i = 0; i < warCount; i++) {
        if (wars[i].active) {
            cout << "War: Player " << wars[i].attacker << " vs Player " << wars[i].defender << "\n";
            hasWars = true;
        }
    }
    if (!hasWars) {
        cout << "No active wars.\n";
    }
}

void Conflict::saveWars() const {
    try {
        ofstream file("wars.txt");
        if (!file.is_open()) {
            throw GameException("Unable to open wars.txt!");
        }
        file << warCount << "\n";
        for (int i = 0; i < warCount; i++) {
            file << wars[i].attacker << " " << wars[i].defender << " "
                << (wars[i].active ? 1 : 0) << "\n";
        }
        file.close();
    }
    catch (const GameException& e) {
        cout << "Error: " << e.message << "\n";
    }
}

void Conflict::loadWars() {
    try {
        ifstream file("wars.txt");
        if (!file.is_open()) {
            throw GameException("Unable to open wars.txt!");
        }
        file >> warCount;
        for (int i = 0; i < warCount; i++) {
            int active;
            file >> wars[i].attacker >> wars[i].defender >> active;
            wars[i].active = (active == 1);
        }
        file.close();
    }
    catch (const GameException& e) {
        cout << "Error: " << e.message << "\n";
    }
}