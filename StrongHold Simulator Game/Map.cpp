#include "Stronghold.h"
#include <iostream>
#include <fstream>
using namespace std;

Map::Map() : positionCount(0) {
    for (int i = 0; i < 10; i++) {
        positions[i].playerId = -1;
        positions[i].x = -1;
        positions[i].y = -1;
    }
}

void Map::movePlayer(int playerId, int newX, int newY) {
    try {
        if (newX < 0 || newX >= 10 || newY < 0 || newY >= 10) {
            throw GameException("Invalid coordinates! Must be within 0-9.");
        }
        if (isPositionOccupied(newX, newY)) {
            throw GameException("Position (" + to_string(newX) + "," + to_string(newY) + ") is already occupied!");
        }
        for (int i = 0; i < positionCount; i++) {
            if (positions[i].playerId == playerId) {
                positions[i].x = newX;
                positions[i].y = newY;
                cout << "Player " << playerId << " moved to (" << newX << "," << newY << ").\n";
                return;
            }
        }
        if (positionCount < 10) {
            positions[positionCount].playerId = playerId;
            positions[positionCount].x = newX;
            positions[positionCount].y = newY;
            positionCount++;
            cout << "Player " << playerId << " placed at (" << newX << "," << newY << ").\n";
        }
        else {
            throw GameException("Map is full!");
        }
    }
    catch (const GameException& e) {
        cout << "Error: " << e.message << "\n";
    }
}

void Map::displayMap() const {
    cout << "\n===== Kingdom Map =====\n";
    char grid[10][10];
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            grid[i][j] = '.';
        }
    }
    for (int i = 0; i < positionCount; i++) {
        if (positions[i].playerId != -1) {
            grid[positions[i].x][positions[i].y] = '0' + positions[i].playerId;
        }
    }
    cout << "  ";
    for (int j = 0; j < 10; j++) {
        cout << j << " ";
    }
    cout << "\n";
    for (int i = 0; i < 10; i++) {
        cout << i << " ";
        for (int j = 0; j < 10; j++) {
            cout << grid[i][j] << " ";
        }
        cout << "\n";
    }
}

bool Map::isPositionOccupied(int x, int y) const {
    for (int i = 0; i < positionCount; i++) {
        if (positions[i].x == x && positions[i].y == y && positions[i].playerId != -1) {
            return true;
        }
    }
    return false;
}

void Map::saveMap() const {
    try {
        ofstream file("map_save.txt");
        if (!file.is_open()) {
            throw GameException("Unable to open map_save.txt!");
        }
        file << positionCount << "\n";
        for (int i = 0; i < positionCount; i++) {
            file << positions[i].playerId << " " << positions[i].x << " " << positions[i].y << "\n";
        }
        file.close();
    }
    catch (const GameException& e) {
        cout << "Error: " << e.message << "\n";
    }
}

void Map::loadMap() {
    try {
        ifstream file("map_save.txt");
        if (!file.is_open()) {
            throw GameException("Unable to open map_save.txt!");
        }
        file >> positionCount;
        for (int i = 0; i < positionCount; i++) {
            file >> positions[i].playerId >> positions[i].x >> positions[i].y;
        }
        file.close();
    }
    catch (const GameException& e) {
        cout << "Error: " << e.message << "\n";
    }
}

void Map::removePlayer(int playerId) {
    for (int i = 0; i < positionCount; i++) {
        if (positions[i].playerId == playerId) {
            positions[i].playerId = -1;
            positions[i].x = -1;
            positions[i].y = -1;
            for (int j = i; j < positionCount - 1; j++) {
                positions[j] = positions[j + 1];
            }
            positionCount--;
            cout << "Player " << playerId << " removed from the map.\n";
            break;
        }
    }
}