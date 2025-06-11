#include "Stronghold.h"
#include <iostream>
using namespace std;

Army::Army() : soldiers(10), morale(100) {}

void Army::train(ResourceManager& rm) {
    int foodCost = 10;
    int goldCost = 5;
    if (rm.hasResource("Food", foodCost) && rm.hasResource("Gold", goldCost)) {
        rm.consume("Food", foodCost, 0); // playerId 0 for single-player compatibility
        rm.consume("Gold", goldCost, 0);
        soldiers += 5;
        morale += 2;
        cout << "Trained 5 soldiers. Current strength: " << soldiers << ". Morale increased to " << morale << ".\n";
    }
    else {
        cout << "Not enough resources to train. Required: " << foodCost << " Food and " << goldCost << " Gold.\n";
    }
}

void Army::pay(ResourceManager& rm) {
    int payment = soldiers * 2;
    if (rm.hasResource("Gold", payment)) {
        rm.consume("Gold", payment, 0);
        morale += 5;
        if (morale > 100) morale = 100;
        cout << "Army paid " << payment << " gold. Morale boosted to " << morale << ".\n";
    }
    else {
        cout << "Insufficient gold (" << payment << " needed). Morale decreased!\n";
        morale -= 10;
        if (morale < 0) morale = 0;
        cout << "New morale: " << morale << endl;
    }
}

void Army::display() const {
    cout << "Army Status:\n";
    cout << "  Soldiers: " << soldiers << endl;
    cout << "  Morale: " << morale << "%" << endl;
    cout << "  Army strength: ";
    if (morale > 80) {
        cout << "Excellent";
    }
    else if (morale > 60) {
        cout << "Good";
    }
    else if (morale > 40) {
        cout << "Average";
    }
    else if (morale > 20) {
        cout << "Poor";
    }
    else {
        cout << "Critical - desertion risk!";
    }
    cout << endl;
}

void Army::setSoldiers(int newSoldiers) {
    soldiers = newSoldiers;
    if (soldiers < 0) soldiers = 0;
}

void Army::setMorale(int newMorale) {
    morale = newMorale;
    if (morale < 0) morale = 0;
    if (morale > 100) morale = 100;
}