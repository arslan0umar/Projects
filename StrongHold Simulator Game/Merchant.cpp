#include "Stronghold.h"
#include <iostream>
using namespace std;

void Merchant::updateSatisfaction(int change) {
    satisfaction += change * 2;
    if (satisfaction < 0) satisfaction = 0;
    if (satisfaction > 100) satisfaction = 100;
}

int Merchant::checkEmigration(Population& pop) {
    int emigrants = 0;
    if (satisfaction < 10) {
        emigrants = pop.getTotal() / 8; // 12.5% leave
        cout << "CRISIS: Merchants are closing shops and leaving!\n";
    }
    else if (satisfaction < 25) {
        emigrants = pop.getTotal() / 15; // ~6.7% leave
        cout << "WARNING: Several merchants are relocating!\n";
    }
    else if (satisfaction < 40) {
        emigrants = pop.getTotal() / 25; // 4% leave
        cout << "Some merchants have moved for better trade.\n";
    }
    if (emigrants > 0 && emigrants < 1) emigrants = 1;
    if (emigrants > 0) {
        pop.modify(-emigrants);
    }
    return emigrants;
}

void Merchant::displayStatus() const {
    cout << "Merchant Status:\n";
    cout << "  Satisfaction: " << satisfaction << "%\n";
    if (satisfaction < 20) {
        cout << "  Warning: Merchants are leaving the kingdom!\n";
    }
    else if (satisfaction < 40) {
        cout << "  Merchants are concerned about trade conditions.\n";
    }
    else if (satisfaction < 60) {
        cout << "  Merchants are conducting business as usual.\n";
    }
    else if (satisfaction < 80) {
        cout << "  Merchants are prospering under your rule.\n";
    }
    else {
        cout << "  The merchant class is thriving and expanding.\n";
    }
}