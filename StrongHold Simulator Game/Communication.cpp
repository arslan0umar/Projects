#include "Stronghold.h"
#include <iostream>
#include <fstream>
using namespace std;

Communication::Communication() : messageCount(0) {
    for (int i = 0; i < 50; i++) {
        messages[i] = "";
    }
}

void Communication::sendMessage(int senderId, int receiverId, const string& message) {
    try {
        if (messageCount >= 50) {
            throw GameException("Message log is full!");
        }
        string formattedMessage = "From Player " + to_string(senderId) + " to Player " +
            to_string(receiverId) + ": " + message;
        messages[messageCount++] = formattedMessage;
        cout << "Message sent successfully.\n";
    }
    catch (const GameException& e) {
        cout << "Error: " << e.message << "\n";
    }
}

void Communication::displayMessages(int playerId) const {
    cout << "\n===== Messages for Player " << playerId << " =====\n";
    bool hasMessages = false;
    for (int i = 0; i < messageCount; i++) {
        if (messages[i].find("to Player " + to_string(playerId) + ":") != string::npos ||
            messages[i].find("From Player " + to_string(playerId) + " ") != string::npos) {
            cout << messages[i] << "\n";
            hasMessages = true;
        }
    }
    if (!hasMessages) {
        cout << "No messages found.\n";
    }
}

void Communication::saveMessages() const {
    try {
        ofstream file("messages.txt");
        if (!file.is_open()) {
            throw GameException("Unable to open messages.txt!");
        }
        file << messageCount << "\n";
        for (int i = 0; i < messageCount; i++) {
            file << messages[i] << "\n";
        }
        file.close();
    }
    catch (const GameException& e) {
        cout << "Error: " << e.message << "\n";
    }
}

void Communication::loadMessages() {
    try {
        ifstream file("messages.txt");
        if (!file.is_open()) {
            throw GameException("Unable to open messages.txt!");
        }
        file >> messageCount;
        file.ignore();
        for (int i = 0; i < messageCount; i++) {
            getline(file, messages[i]);
        }
        file.close();
    }
    catch (const GameException& e) {
        cout << "Error: " << e.message << "\n";
    }
}