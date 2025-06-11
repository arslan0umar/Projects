#include "Stronghold.h"
#include <iostream>
using namespace std;

ResourceManager::ResourceManager() {
    resourceCount = 5;
    resourceNames[0] = "Food";
    resourceNames[1] = "Wood";
    resourceNames[2] = "Stone";
    resourceNames[3] = "Iron";
    resourceNames[4] = "Gold";
    for (int i = 0; i < resourceCount; i++) {
        resourceValues[i] = 100;
    }
}

void ResourceManager::gather(string resource, int amount, int playerId) {
    int index = findResourceIndex(resource);
    if (index != -1) {
        resourceValues[index] += amount;
        cout << "Player " << playerId << " gathered " << amount << " " << resource << ".\n";
    }
    else {
        throw GameException("Resource not found: " + resource);
    }
}

void ResourceManager::consume(string resource, int amount, int playerId) {
    int index = findResourceIndex(resource);
    if (index != -1) {
        if (resourceValues[index] >= amount) {
            resourceValues[index] -= amount;
            cout << "Player " << playerId << " consumed " << amount << " " << resource << ".\n";
        }
        else {
            throw GameException("Not enough " + resource + ". Available: " + to_string(resourceValues[index]));
        }
    }
    else {
        throw GameException("Resource not found: " + resource);
    }
}

void ResourceManager::trade(string from, string to, int rate) {
    int fromIndex = findResourceIndex(from);
    int toIndex = findResourceIndex(to);
    if (fromIndex != -1 && toIndex != -1) {
        if (resourceValues[fromIndex] >= rate) {
            resourceValues[fromIndex] -= rate;
            resourceValues[toIndex] += rate;
            cout << "Traded " << rate << " " << from << " for " << to << ".\n";
        }
        else {
            throw GameException("Not enough " + from + ". Available: " + to_string(resourceValues[fromIndex]));
        }
    }
    else {
        throw GameException("One or both resources not found.");
    }
}

void ResourceManager::display() const {
    cout << "Resource Status:\n";
    for (int i = 0; i < resourceCount; i++) {
        cout << i + 1 << ". " << resourceNames[i] << ": " << resourceValues[i] << "\n";
    }
}

bool ResourceManager::hasResource(const string& name, int amount) {
    int index = findResourceIndex(name);
    return (index != -1 && resourceValues[index] >= amount);
}

int ResourceManager::findResourceIndex(const string& name) const {
    for (int i = 0; i < resourceCount; i++) {
        if (resourceNames[i] == name) {
            return i;
        }
    }
    return -1;
}