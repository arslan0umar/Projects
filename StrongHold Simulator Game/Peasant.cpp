#include "Stronghold.h"
#include <iostream>
using namespace std;

void Peasant::updateSatisfaction(int change) {
    satisfaction += change;
    if (satisfaction < 0) satisfaction = 0;
    if (satisfaction > 100) satisfaction = 100;
}

int Peasant::checkEmigration(Population& pop) {
    int emigrants = 0;
    if (satisfaction < 10) {
        emigrants = pop.getTotal() / 5; // 20% leave
        cout << "CRISIS: Many peasants are leaving due to extreme dissatisfaction!\n";
    }
    else if (satisfaction < 25) {
        emigrants = pop.getTotal() / 10; // 10% leave
        cout << "WARNING: Several peasants are leaving due to poor conditions!\n";
    }
    else if (satisfaction < 40) {
        emigrants = pop.getTotal() / 20; // 5% leave
        cout << "Some peasants have left seeking better lives elsewhere.\n";
    }
    if (emigrants > 0 && emigrants < 1) emigrants = 1;
    if (emigrants > 0) {
        pop.modify(-emigrants);
    }
    return emigrants;
}

void Peasant::displayStatus() const {
    cout << "Peasant Status:\n";
    cout << "  Satisfaction: " << satisfaction << "%\n";
    if (satisfaction < 20) {
        cout << "  Warning: Peasants are close to rebellion!\n";
    }
    else if (satisfaction < 40) {
        cout << "  Peasants are discontent and grumbling.\n";
    }
    else if (satisfaction < 60) {
        cout << "  Peasants are acceptably content.\n";
    }
    else if (satisfaction < 80) {
        cout << "  Peasants are satisfied with their conditions.\n";
    }
    else {
        cout << "  Peasants are extremely happy with your rule.\n";
    }
}