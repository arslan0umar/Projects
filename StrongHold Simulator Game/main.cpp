#include <iostream>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include "Stronghold.h"
using namespace std;

void displayBorder(int width = 60, char symbol = '-') {
    cout << string(width, symbol) << endl;
}

void displayCenteredText(const string& text, int width = 60, char borderChar = '|') {
    int textLength = static_cast<int>(text.length());
    int padding = (width - textLength - 2) / 2;
    cout << borderChar << string(padding, ' ') << text;
    if ((width - textLength) % 2 != 0) {
        cout << string(padding + 1, ' ');
    }
    else {
        cout << string(padding, ' ');
    }
    cout << borderChar << endl;
}

void displayTitleBox(const string& title, const string& subtitle = "") {
    displayBorder(60, '=');
    displayCenteredText(title, 60, '|');
    if (!subtitle.empty()) {
        displayCenteredText(subtitle, 60, '|');
    }
    displayBorder(60, '=');
}

void displayKingdomStatus(const ResourceManager& rm, const Population& pop,
    const Army& army, const Bank& bank, const Leader& leader,
    const Peasant& peasant, const Merchant& merchant, const Noble& noble) {
    displayTitleBox("KINGDOM STATUS");
    cout << "\n--- LEADERSHIP ---\n";
    leader.display();
    cout << "\n--- POPULATION ---\n";
    pop.display();
    cout << "\n--- SOCIAL CLASSES ---\n";
    peasant.displayStatus();
    merchant.displayStatus();
    noble.displayStatus();
    cout << "\n--- MILITARY ---\n";
    army.display();
    cout << "\n--- ECONOMY ---\n";
    bank.display();
    cout << "\n--- RESOURCES ---\n";
    rm.display();
    displayBorder();
}

void gatherResources(ResourceManager& rm, int playerId) {
    displayTitleBox("RESOURCE GATHERING");
    cout << "\nAvailable resources:\n";
    rm.display();
    cout << "\nEnter resource to gather (or 'back' to return): ";
    string name;
    cin >> name;
    if (name == "back") return;
    cout << "Enter amount to gather: ";
    int amount;
    cin >> amount;
    try {
        if (amount <= 0) {
            throw GameException("Amount must be positive.");
        }
        rm.gather(name, amount, playerId);
    }
    catch (const GameException& e) {
        cout << "Error: " << e.message << "\n";
    }
}

void consumeResources(ResourceManager& rm, int playerId) {
    displayTitleBox("RESOURCE CONSUMPTION");
    cout << "\nAvailable resources:\n";
    rm.display();
    cout << "\nEnter resource to consume (or 'back' to return): ";
    string name;
    cin >> name;
    if (name == "back") return;
    cout << "Enter amount to consume: ";
    int amount;
    cin >> amount;
    try {
        if (amount <= 0) {
            throw GameException("Amount must be positive.");
        }
        rm.consume(name, amount, playerId);
    }
    catch (const GameException& e) {
        cout << "Error: " << e.message << "\n";
    }
}

void tradeResources(ResourceManager& rm) {
    displayTitleBox("RESOURCE TRADING");
    cout << "\nAvailable resources:\n";
    rm.display();
    cout << "\nEnter resource to trade from: ";
    string fromResource;
    cin >> fromResource;
    cout << "Enter resource to trade to: ";
    string toResource;
    cin >> toResource;
    cout << "Enter amount to trade: ";
    int amount;
    cin >> amount;
    try {
        if (amount <= 0) {
            throw GameException("Amount must be positive.");
        }
        rm.trade(fromResource, toResource, amount);
    }
    catch (const GameException& e) {
        cout << "Error: " << e.message << "\n";
    }
}

struct Player {
    ResourceManager rm;
    Army army;
    Bank bank;
    Economy economy;
    Leader leader;
    Population population;
    Peasant peasant;
    Merchant merchant;
    Noble noble;
    int id;
    string name;
    bool isActive; // Added to track defeated kingdoms
    Player() : id(0), isActive(true) {} // Initialize isActive to true
};

int main() {
    srand(static_cast<unsigned int>(time(0)));
    displayTitleBox("STRONGHOLD GAME", "Medieval Kingdom Simulator");

    // Get number of players
    int numPlayers;
    do {
        cout << "Enter number of players (1-10): ";
        cin >> numPlayers;
        if (numPlayers < 1 || numPlayers > 10) {
            cout << "Please enter a number between 1 and 10.\n";
        }
    } while (numPlayers < 1 || numPlayers > 10);
    cin.ignore(); // Clear newline

    // Initialize players
    Player players[10]; // Fixed-size array
    Communication comm;
    Diplomacy diplomacy;
    Market market;
    Conflict conflict;
    Map map;
    EventManager eventManager;

    // Get player names and initialize kingdoms
    for (int i = 0; i < numPlayers; i++) {
        cout << "Enter name for Player " << (i + 1) << ": ";
        string playerName;
        getline(cin, playerName);
        if (playerName.empty()) {
            playerName = "Player " + to_string(i + 1);
        }
        players[i].id = i + 1;
        players[i].name = playerName;
        players[i].leader.setName(playerName);
        // Random map placement
        int x, y;
        bool validPosition;
        do {
            x = rand() % 10;
            y = rand() % 10;
            validPosition = !map.isPositionOccupied(x, y);
        } while (!validPosition);
        map.movePlayer(players[i].id, x, y);
        cout << "Kingdom for " << playerName << " (Player " << players[i].id << ") formed at (" << x << ", " << y << ").\n";
    }

    bool running = true;
    int turn = 1;

    while (running) {
        int activePlayers = 0;
        for (int i = 0; i < numPlayers; i++) {
            if (!players[i].isActive) continue; // Skip defeated players
            activePlayers++;
            displayBorder();
            displayTitleBox("TURN " + to_string(turn) + " - " + players[i].name + "'s Realm",
                "Leader: " + players[i].leader.getName());
            displayTitleBox("STRONGHOLD GAME MENU");
            cout << "1. Display Kingdom Status\n";
            cout << "2. Resource Management\n";
            cout << "3. Population Management\n";
            cout << "4. Military Management\n";
            cout << "5. Economy Management\n";
            cout << "6. Leadership Actions\n";
            cout << "7. Banking System\n";
            cout << "8. Trigger Random Event\n";
            cout << "9. Communication\n";
            cout << "10. Diplomacy\n";
            cout << "11. Market\n";
            cout << "12. Conflict\n";
            cout << "13. Map\n";
            cout << "14. Save Game\n";
            cout << "15. Load Game\n";
            cout << "0. Exit Game\n";
            cout << "\nEnter your choice: ";

            int choice;
            cin >> choice;

            try {
                switch (choice) {
                case 1:
                    displayKingdomStatus(players[i].rm, players[i].population, players[i].army,
                        players[i].bank, players[i].leader, players[i].peasant,
                        players[i].merchant, players[i].noble);
                    break;

                case 2: {
                    displayTitleBox("RESOURCE MANAGEMENT");
                    cout << "1. Gather Resources\n";
                    cout << "2. Consume Resources\n";
                    cout << "3. Trade Resources\n";
                    cout << "4. Display Resources\n";
                    cout << "Enter choice: ";
                    int resourceChoice;
                    cin >> resourceChoice;
                    switch (resourceChoice) {
                    case 1: gatherResources(players[i].rm, players[i].id); break;
                    case 2: consumeResources(players[i].rm, players[i].id); break;
                    case 3: tradeResources(players[i].rm); break;
                    case 4:
                        displayTitleBox("RESOURCES");
                        players[i].rm.display();
                        break;
                    default: throw GameException("Invalid resource management choice.");
                    }
                    break;
                }

                case 3: {
                    displayTitleBox("POPULATION MANAGEMENT");
                    cout << "1. View Population\n";
                    cout << "2. Modify Population\n";
                    cout << "3. View Social Class Status\n";
                    cout << "Enter choice: ";
                    int popChoice;
                    cin >> popChoice;
                    switch (popChoice) {
                    case 1:
                        displayTitleBox("POPULATION STATUS");
                        players[i].population.display();
                        break;
                    case 2: {
                        cout << "Enter amount to increase (+) or decrease (-): ";
                        int amount;
                        cin >> amount;
                        players[i].population.modify(amount);
                        break;
                    }
                    case 3:
                        displayTitleBox("SOCIAL CLASS STATUS");
                        players[i].peasant.displayStatus();
                        players[i].merchant.displayStatus();
                        players[i].noble.displayStatus();
                        break;
                    default: throw GameException("Invalid population management choice.");
                    }
                    break;
                }

                case 4: {
                    displayTitleBox("MILITARY MANAGEMENT");
                    cout << "1. Train Army\n";
                    cout << "2. Pay Army\n";
                    cout << "3. Display Army Status\n";
                    cout << "Enter choice: ";
                    int militaryChoice;
                    cin >> militaryChoice;
                    switch (militaryChoice) {
                    case 1:
                        displayTitleBox("ARMY TRAINING");
                        players[i].army.train(players[i].rm);
                        break;
                    case 2:
                        displayTitleBox("ARMY PAYMENT");
                        players[i].army.pay(players[i].rm);
                        break;
                    case 3:
                        displayTitleBox("ARMY STATUS");
                        players[i].army.display();
                        break;
                    default: throw GameException("Invalid military management choice.");
                    }
                    break;
                }

                case 5:
                    displayTitleBox("ECONOMY MANAGEMENT");
                    players[i].economy.manage(players[i].rm, players[i].peasant, players[i].merchant, players[i].noble, players[i].id);
                    break;

                case 6:
                    displayTitleBox("LEADERSHIP ACTIONS");
                    players[i].leader.menu(players[i].population, players[i].army);
                    break;

                case 7:
                    displayTitleBox("BANKING SYSTEM");
                    players[i].bank.menu(players[i].rm, players[i].id);
                    break;

                case 8:
                    displayTitleBox("RANDOM EVENT");
                    eventManager.trigger(players[i].rm, players[i].population, players[i].army,
                        players[i].peasant, players[i].merchant, players[i].noble, players[i].id);
                    break;

                case 9: {
                    displayTitleBox("COMMUNICATION");
                    cout << "1. Send Message\n";
                    cout << "2. View Messages\n";
                    cout << "Enter choice: ";
                    int commChoice;
                    cin >> commChoice;
                    if (commChoice == 1) {
                        cout << "Enter recipient Player ID (1-" << numPlayers << "): ";
                        int receiverId;
                        cin >> receiverId;
                        if (receiverId < 1 || receiverId > numPlayers || receiverId == players[i].id) {
                            throw GameException("Invalid recipient ID.");
                        }
                        cout << "Enter message: ";
                        cin.ignore();
                        string message;
                        getline(cin, message);
                        comm.sendMessage(players[i].id, receiverId, message);
                    }
                    else if (commChoice == 2) {
                        comm.displayMessages(players[i].id);
                    }
                    else {
                        throw GameException("Invalid communication choice.");
                    }
                    break;
                }

                case 10: {
                    displayTitleBox("DIPLOMACY");
                    cout << "1. Form Alliance\n";
                    cout << "2. Break Alliance\n";
                    cout << "3. View Treaties\n";
                    cout << "Enter choice: ";
                    int dipChoice;
                    cin >> dipChoice;
                    if (dipChoice == 1 || dipChoice == 2) {
                        cout << "Enter other Player ID (1-" << numPlayers << "): ";
                        int otherId;
                        cin >> otherId;
                        if (otherId < 1 || otherId > numPlayers || otherId == players[i].id) {
                            throw GameException("Invalid player ID.");
                        }
                        if (dipChoice == 1) {
                            diplomacy.formAlliance(players[i].id, otherId);
                        }
                        else {
                            diplomacy.breakAlliance(players[i].id, otherId);
                        }
                    }
                    else if (dipChoice == 3) {
                        diplomacy.displayTreaties();
                    }
                    else {
                        throw GameException("Invalid diplomacy choice.");
                    }
                    break;
                }

                case 11: {
                    displayTitleBox("MARKET");
                    cout << "1. Trade\n";
                    cout << "2. Smuggle\n";
                    cout << "3. View Transactions\n";
                    cout << "Enter choice: ";
                    int marketChoice;
                    cin >> marketChoice;
                    if (marketChoice == 1) {
                        cout << "Enter recipient Player ID (1-" << numPlayers << "): ";
                        int receiverId;
                        cin >> receiverId;
                        if (receiverId < 1 || receiverId > numPlayers || receiverId == players[i].id) {
                            throw GameException("Invalid recipient ID.");
                        }
                        cout << "Your resources:\n";
                        players[i].rm.display();
                        cout << "Player " << receiverId << "'s resources:\n";
                        players[receiverId - 1].rm.display();
                        cout << "Enter your resource number to trade: ";
                        int resourceNum1;
                        cin >> resourceNum1;
                        cout << "Enter their resource number to receive: ";
                        int resourceNum2;
                        cin >> resourceNum2;
                        cout << "Enter amount to trade: ";
                        int amount;
                        cin >> amount;
                        market.trade(players[i].rm, players[receiverId - 1].rm, resourceNum1, resourceNum2, amount, players[i].id, receiverId);
                    }
                    else if (marketChoice == 2) {
                        cout << "Enter recipient Player ID (1-" << numPlayers << "): ";
                        int receiverId;
                        cin >> receiverId;
                        if (receiverId < 1 || receiverId > numPlayers || receiverId == players[i].id) {
                            throw GameException("Invalid recipient ID.");
                        }
                        cout << "Enter resource to smuggle: ";
                        string resource;
                        cin >> resource;
                        cout << "Enter amount: ";
                        int amount;
                        cin >> amount;
                        market.smuggle(players[i].rm, players[receiverId - 1].rm, resource, amount, players[i].id, receiverId);
                    }
                    else if (marketChoice == 3) {
                        market.displayMarket();
                    }
                    else {
                        throw GameException("Invalid market choice.");
                    }
                    break;
                }

                case 12: {
                    displayTitleBox("CONFLICT");
                    cout << "1. Declare War\n";
                    cout << "2. Betray Ally\n";
                    cout << "3. View Wars\n";
                    cout << "Enter choice: ";
                    int conflictChoice;
                    cin >> conflictChoice;
                    if (conflictChoice == 1 || conflictChoice == 2) {
                        cout << "Enter target Player ID (1-" << numPlayers << "): ";
                        int targetId;
                        cin >> targetId;
                        if (targetId < 1 || targetId > numPlayers || targetId == players[i].id) {
                            throw GameException("Invalid target ID.");
                        }
                        if (!players[targetId - 1].isActive) {
                            throw GameException("Cannot target a defeated kingdom!");
                        }
                        if (conflictChoice == 1) {
                            conflict.declareWar(players[i].id, targetId, players[i].army, players[targetId - 1].army,
                                players[i].leader, players[targetId - 1].leader,
                                players[i].peasant, players[i].merchant, players[i].noble,
                                players[targetId - 1].peasant, players[targetId - 1].merchant, players[targetId - 1].noble);
                        }
                        else {
                            conflict.betray(players[i].id, targetId, diplomacy, players[i].army, players[targetId - 1].army,
                                players[i].leader, players[targetId - 1].leader,
                                players[i].peasant, players[i].merchant, players[i].noble,
                                players[targetId - 1].peasant, players[targetId - 1].merchant, players[targetId - 1].noble);
                        }
                    }
                    else if (conflictChoice == 3) {
                        conflict.displayWars();
                    }
                    else {
                        throw GameException("Invalid conflict choice.");
                    }
                    break;
                }

                case 13: {
                    displayTitleBox("MAP");
                    cout << "1. Move Position\n";
                    cout << "2. View Map\n";
                    cout << "Enter choice: ";
                    int mapChoice;
                    cin >> mapChoice;
                    if (mapChoice == 1) {
                        cout << "Enter new X coordinate (0-9): ";
                        int x;
                        cin >> x;
                        cout << "Enter new Y coordinate (0-9): ";
                        int y;
                        cin >> y;
                        map.movePlayer(players[i].id, x, y);
                    }
                    else if (mapChoice == 2) {
                        map.displayMap();
                    }
                    else {
                        throw GameException("Invalid map choice.");
                    }
                    break;
                }

                case 14: {
                    displayTitleBox("SAVE GAME");
                    for (int j = 0; j < numPlayers; j++) {
                        Logger::saveGame(players[j].population, players[j].army, players[j].bank,
                            players[j].leader, players[j].rm, players[j].economy,
                            players[j].peasant, players[j].merchant, players[j].noble);
                    }
                    comm.saveMessages();
                    diplomacy.saveTreaties();
                    market.saveMarket();
                    conflict.saveWars();
                    map.saveMap();
                    string scoreInfo = "Session saved on Turn " + to_string(turn) + " for Player " + players[i].name;
                    Logger::logScore(scoreInfo);
                    cout << "Game saved successfully.\n";
                    break;
                }

                case 15: {
                    displayTitleBox("LOAD GAME");
                    bool success = true;
                    for (int j = 0; j < numPlayers; j++) {
                        if (!Logger::loadGame(players[j].population, players[j].army, players[j].bank,
                            players[j].leader, players[j].rm, players[j].economy,
                            players[j].peasant, players[j].merchant, players[j].noble)) {
                            success = false;
                        }
                        // Check population after loading to update isActive
                        if (players[j].population.getTotal() <= 0) {
                            players[j].isActive = false;
                            players[j].noble.updateSatisfaction(-players[j].noble.getSatisfaction());
                            players[j].merchant.updateSatisfaction(-players[j].merchant.getSatisfaction());
                            players[j].peasant.updateSatisfaction(-players[j].peasant.getSatisfaction());
                            players[j].army.setSoldiers(0);
                            map.removePlayer(players[j].id);
                        }
                    }
                    comm.loadMessages();
                    diplomacy.loadTreaties();
                    market.loadMarket();
                    conflict.loadWars();
                    map.loadMap();
                    if (success) {
                        cout << "Game loaded successfully.\n";
                    }
                    else {
                        cout << "Some errors occurred while loading.\n";
                    }
                    break;
                }

                case 0:
                    running = false;
                    break;

                default:
                    throw GameException("Invalid choice!");
                }

                if (running && choice != 14 && choice != 15) {
                    players[i].population.naturalChange();
                    players[i].peasant.checkEmigration(players[i].population);
                    players[i].merchant.checkEmigration(players[i].population);
                    players[i].noble.checkEmigration(players[i].population);
                    if (players[i].population.getTotal() <= 0) {
                        displayTitleBox("KINGDOM COLLAPSED");
                        cout << "Player " << players[i].name << "'s kingdom has collapsed due to zero population!\n";
                        players[i].isActive = false;
                        players[i].noble.updateSatisfaction(-players[i].noble.getSatisfaction());
                        players[i].merchant.updateSatisfaction(-players[i].merchant.getSatisfaction());
                        players[i].peasant.updateSatisfaction(-players[i].peasant.getSatisfaction());
                        players[i].army.setSoldiers(0);
                        map.removePlayer(players[i].id);
                        Logger::logScore("Player " + players[i].name + " lost due to zero population on turn " + to_string(turn));
                        continue;
                    }
                    if (!players[i].rm.hasResource("Food", 10)) {
                        displayTitleBox("WARNING");
                        cout << "Food supplies are critically low!\n";
                        players[i].population.modify(-5);
                        players[i].peasant.updateSatisfaction(-5);
                        if (players[i].population.getTotal() <= 0) {
                            displayTitleBox("KINGDOM COLLAPSED");
                            cout << "Player " << players[i].name << "'s kingdom has collapsed due to starvation!\n";
                            players[i].isActive = false;
                            players[i].noble.updateSatisfaction(-players[i].noble.getSatisfaction());
                            players[i].merchant.updateSatisfaction(-players[i].merchant.getSatisfaction());
                            players[i].peasant.updateSatisfaction(-players[i].peasant.getSatisfaction());
                            players[i].army.setSoldiers(0);
                            map.removePlayer(players[i].id);
                            Logger::logScore("Player " + players[i].name + " lost due to zero population on turn " + to_string(turn));
                            continue;
                        }
                    }
                    if (rand() % 100 < 20) {
                        displayTitleBox("RANDOM EVENT");
                        cout << "A minor event occurred...\n";
                        int minorEvent = rand() % 3;
                        switch (minorEvent) {
                        case 0:
                            cout << "Some trade caravans arrived, bringing extra gold!\n";
                            players[i].rm.gather("Gold", 15, players[i].id);
                            break;
                        case 1:
                            cout << "Local farmers had a good harvest this month.\n";
                            players[i].rm.gather("Food", 20, players[i].id);
                            break;
                        case 2:
                            cout << "Craftsmen produced extra building materials.\n";
                            players[i].rm.gather("Wood", 15, players[i].id);
                            players[i].rm.gather("Stone", 10, players[i].id);
                            break;
                        }
                    }
                }
            }
            catch (const GameException& e) {
                cout << "Error: " << e.message << "\n";
            }
        }
        // Check for game over conditions
        if (numPlayers > 1 && activePlayers <= 1) {
            // Only end game due to victory/defeat in multiplayer mode
            running = false;
            if (activePlayers == 1) {
                for (int i = 0; i < numPlayers; i++) {
                    if (players[i].isActive) {
                        displayTitleBox("VICTORY");
                        cout << "Player " << players[i].name << " is the last kingdom standing!\n";
                        Logger::logScore("Player " + players[i].name + " won on turn " + to_string(turn));
                        break;
                    }
                }
            }
            else {
                displayTitleBox("GAME OVER");
                cout << "All kingdoms have collapsed! No victor.\n";
            }
        }
        else if (numPlayers == 1 && activePlayers == 0) {
            // In single player mode, only end if the player is defeated
            running = false;
            displayTitleBox("GAME OVER");
            cout << "Your kingdom has collapsed!\n";
        }
        if (running) {
            turn++;
        }
    }

    displayTitleBox("GAME OVER");
    for (int i = 0; i < numPlayers; i++) {
        cout << "\nPlayer " << players[i].id << " (" << players[i].name << ") Final score: " << players[i].population.getTotal() * 10 << "\n";
        string finalScoreInfo = "Final score for Player " + players[i].name + ": " +
            to_string(players[i].population.getTotal() * 10) + " after " + to_string(turn) + " turns";
        Logger::logScore(finalScoreInfo);
    }
    cout << "\nThank you for playing Stronghold!\n";

    return 0;
}