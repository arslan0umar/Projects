#include "Stronghold.h"
#include <iostream>
#include <fstream>
using namespace std;

Diplomacy::Diplomacy() : treatyCount(0) {
    for (int i = 0; i < 20; i++) {
        treaties[i].player1 = -1;
        treaties[i].player2 = -1;
        treaties[i].active = false;
    }
}

void Diplomacy::formAlliance(int player1, int player2) {
    try {
        if (player1 == player2) {
            throw GameException("Cannot form alliance with yourself!");
        }
        if (hasAlliance(player1, player2)) {
            throw GameException("Alliance already exists between Player " + to_string(player1) +
                " and Player " + to_string(player2) + "!");
        }
        if (treatyCount >= 20) {
            throw GameException("Treaty log is full!");
        }
        treaties[treatyCount].player1 = player1;
        treaties[treatyCount].player2 = player2;
        treaties[treatyCount].active = true;
        treatyCount++;
        cout << "Alliance formed between Player " << player1 << " and Player " << player2 << ".\n";
    }
    catch (const GameException& e) {
        cout << "Error: " << e.message << "\n";
    }
}

void Diplomacy::breakAlliance(int player1, int player2) {
    try {
        for (int i = 0; i < treatyCount; i++) {
            if ((treaties[i].player1 == player1 && treaties[i].player2 == player2) ||
                (treaties[i].player1 == player2 && treaties[i].player2 == player1)) {
                if (treaties[i].active) {
                    treaties[i].active = false;
                    cout << "Alliance broken between Player " << player1 << " and Player " << player2 << ".\n";
                    return;
                }
            }
        }
        throw GameException("No active alliance found between Player " + to_string(player1) +
            " and Player " + to_string(player2) + "!");
    }
    catch (const GameException& e) {
        cout << "Error: " << e.message << "\n";
    }
}

bool Diplomacy::hasAlliance(int player1, int player2) const {
    for (int i = 0; i < treatyCount; i++) {
        if (((treaties[i].player1 == player1 && treaties[i].player2 == player2) ||
            (treaties[i].player1 == player2 && treaties[i].player2 == player1)) &&
            treaties[i].active) {
            return true;
        }
    }
    return false;
}

void Diplomacy::displayTreaties() const {
    cout << "\n===== Current Treaties =====\n";
    bool hasTreaties = false;
    for (int i = 0; i < treatyCount; i++) {
        if (treaties[i].active) {
            cout << "Alliance between Player " << treaties[i].player1 << " and Player "
                << treaties[i].player2 << "\n";
            hasTreaties = true;
        }
    }
    if (!hasTreaties) {
        cout << "No active treaties.\n";
    }
}

void Diplomacy::saveTreaties() const {
    try {
        ofstream file("treaties.txt");
        if (!file.is_open()) {
            throw GameException("Unable to open treaties.txt!");
        }
        file << treatyCount << "\n";
        for (int i = 0; i < treatyCount; i++) {
            file << treaties[i].player1 << " " << treaties[i].player2 << " "
                << (treaties[i].active ? 1 : 0) << "\n";
        }
        file.close();
    }
    catch (const GameException& e) {
        cout << "Error: " << e.message << "\n";
    }
}

void Diplomacy::loadTreaties() {
    try {
        ifstream file("treaties.txt");
        if (!file.is_open()) {
            throw GameException("Unable to open treaties.txt!");
        }
        file >> treatyCount;
        for (int i = 0; i < treatyCount; i++) {
            int active;
            file >> treaties[i].player1 >> treaties[i].player2 >> active;
            treaties[i].active = (active == 1);
        }
        file.close();
    }
    catch (const GameException& e) {
        cout << "Error: " << e.message << "\n";
    }
}