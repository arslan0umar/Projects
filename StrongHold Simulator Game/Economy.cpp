#include "Stronghold.h"
#include <iostream>
using namespace std;

Economy::Economy() : taxRate(10) {}

void Economy::manage(ResourceManager& rm, Peasant& peasant, Merchant& merchant, Noble& noble, int playerId) {
    int choice;
    cout << "\n===== Economy Management =====\n";
    cout << "1. Set Tax Rate\n";
    cout << "2. Collect Taxes\n";
    cout << "3. Invest in Economy\n";
    cout << "4. Back to Main Menu\n";
    cout << "Enter choice: ";
    cin >> choice;

    switch (choice) {
    case 1: {
        cout << "Current tax rate: " << taxRate << "%\n";
        cout << "Enter new tax rate (1-50): ";
        int newRate;
        cin >> newRate;
        if (newRate >= 1 && newRate <= 50) {
            int oldRate = taxRate;
            taxRate = newRate;
            int change = oldRate - newRate; // Positive if taxes went down
            peasant.updateSatisfaction(change * 2);
            merchant.updateSatisfaction(change);
            noble.updateSatisfaction(change / 2);
            cout << "Tax rate changed from " << oldRate << "% to " << taxRate << "%.\n";
            peasant.displayStatus();
            merchant.displayStatus();
            noble.displayStatus();
        }
        else {
            cout << "Invalid tax rate. Must be between 1 and 50.\n";
        }
        break;
    }
    case 2: {
        int peasantTax = 1 * taxRate;
        int merchantTax = 3 * taxRate;
        int nobleTax = 5 * taxRate;
        int totalTax = peasantTax + merchantTax + nobleTax;
        rm.gather("Gold", totalTax, playerId);
        cout << "Collected taxes:\n";
        cout << "  From peasants: " << peasantTax << " gold\n";
        cout << "  From merchants: " << merchantTax << " gold\n";
        cout << "  From nobles: " << nobleTax << " gold\n";
        cout << "  Total: " << totalTax << " gold\n";
        peasant.updateSatisfaction(-1);
        merchant.updateSatisfaction(-1);
        noble.updateSatisfaction(-1);
        break;
    }
    case 3: {
        int cost = 50;
        if (rm.hasResource("Gold", cost)) {
            rm.consume("Gold", cost, playerId);
            cout << "Invested " << cost << " gold in the economy.\n";
            peasant.updateSatisfaction(5);
            merchant.updateSatisfaction(10);
            noble.updateSatisfaction(3);
            rm.gather("Food", 20, playerId);
            rm.gather("Wood", 15, playerId);
            rm.gather("Stone", 10, playerId);
            rm.gather("Iron", 5, playerId);
            cout << "Economic growth has increased resources and satisfaction.\n";
        }
        else {
            cout << "Not enough gold to invest in the economy.\n";
        }
        break;
    }
    case 4:
        cout << "Returning to main menu.\n";
        break;
    default:
        cout << "Invalid option.\n";
    }
}

void Economy::display() const {
    cout << "Economy Status:\n";
    cout << "  Current tax rate: " << taxRate << "%\n";
}

void Economy::setTaxRate(int newRate) {
    taxRate = newRate;
    if (taxRate < 0) taxRate = 0;
    if (taxRate > 50) taxRate = 50;
}