#ifndef STRONGHOLD_H
#define STRONGHOLD_H

#include <string>
using namespace std;

class GameException {
public:
    string message;
    GameException(const string& msg) : message(msg) {}
    GameException(const GameException& other) : message(other.message) {}
};

template <typename T>
class Container {
private:
    T items[10];
    int count;
public:
    Container() : count(0) {}
    void add(T item) {
        if (count < 10) {
            items[count++] = item;
        }
        else {
            throw GameException("Container is full!");
        }
    }
    T get(int index) const {
        if (index >= 0 && index < count) {
            return items[index];
        }
        throw GameException("Invalid container index!");
    }
    int size() const { return count; }
};

class ResourceManager;
class Population;
class Army;

class SocialClass {
public:
    SocialClass() : satisfaction(50) {}
    virtual void updateSatisfaction(int change) = 0;
    virtual void displayStatus() const = 0;
    virtual int checkEmigration(Population& pop) = 0;
    int getSatisfaction() const { return satisfaction; }
protected:
    int satisfaction;
};

class Peasant : public SocialClass {
public:
    void updateSatisfaction(int change) override;
    void displayStatus() const override;
    int checkEmigration(Population& pop) override;
};

class Merchant : public SocialClass {
public:
    void updateSatisfaction(int change) override;
    void displayStatus() const override;
    int checkEmigration(Population& pop) override;
};

class Noble : public SocialClass {
public:
    void updateSatisfaction(int change) override;
    void displayStatus() const override;
    int checkEmigration(Population& pop) override;
};

class ResourceManager {
public:
    ResourceManager();
    void gather(string resource, int amount, int playerId);
    void consume(string resource, int amount, int playerId);
    void trade(string from, string to, int rate);
    void display() const;
    bool hasResource(const string& name, int amount);
    string getResourceName(int index) const { return resourceNames[index]; }
    int getResourceValue(int index) const { return resourceValues[index]; }
    int getResourceCount() const { return resourceCount; }
    int findResourceIndex(const string& name) const;
private:
    string resourceNames[10];
    int resourceValues[10];
    int resourceCount;
};

class Army {
public:
    Army();
    void train(ResourceManager& rm);
    void pay(ResourceManager& rm);
    void display() const;
    int getSoldiers() const { return soldiers; }
    int getMorale() const { return morale; }
    void setSoldiers(int newSoldiers);
    void setMorale(int newMorale);
private:
    int soldiers;
    int morale;
};

class Bank {
public:
    Bank();
    void menu(ResourceManager& rm, int playerId);
    int getLoan() const { return loan; }
    void setLoan(int newLoan);
    void display() const;
private:
    int loan;
};

class Economy {
public:
    Economy();
    void manage(ResourceManager& rm, Peasant& peasant, Merchant& merchant, Noble& noble, int playerId);
    void display() const;
    int getTaxRate() const { return taxRate; }
    void setTaxRate(int newRate);
private:
    int taxRate;
};

class Population {
public:
    Population();
    void modify(int amt);
    void naturalChange();
    void display() const;
    int getTotal() const;
    void setTotal(int newCount);
    bool isDefeated() const;
private:
    int count;
};

class Leader {
public:
    Leader();
    void menu(Population& pop, Army& army);
    void display() const;
    string getName() const { return name; }
    int getPopularity() const { return popularity; }
    void setName(const string& newName);
    void setPopularity(int newPopularity);
private:
    string name;
    int popularity;
};

class EventManager {
public:
    void trigger(ResourceManager& rm, Population& pop, Army& army, Peasant& peasant,
        Merchant& merchant, Noble& noble, int playerId);
};

class Communication {
public:
    Communication();
    void sendMessage(int senderId, int receiverId, const string& message);
    void displayMessages(int playerId) const;
    void saveMessages() const;
    void loadMessages();
private:
    string messages[50];
    int messageCount;
};

class Diplomacy {
public:
    Diplomacy();
    void formAlliance(int player1, int player2);
    void breakAlliance(int player1, int player2);
    void displayTreaties() const;
    void saveTreaties() const;
    void loadTreaties();
    bool hasAlliance(int player1, int player2) const;
private:
    struct Treaty {
        int player1;
        int player2;
        bool active;
    };
    Treaty treaties[20];
    int treatyCount;
};

class Market {
public:
    Market();
    void trade(ResourceManager& rm1, ResourceManager& rm2, int resourceNum1, int resourceNum2, int amount, int player1, int player2);
    void smuggle(ResourceManager& rm1, ResourceManager& rm2, const string& resource, int amount, int player1, int player2);
    void displayMarket() const;
    void saveMarket() const;
    void loadMarket();
private:
    struct Trade {
        int player1;
        int player2;
        string resource;
        int amount;
        bool isSmuggling;
    };
    Trade trades[50];
    int tradeCount;
};

class Conflict {
public:
    Conflict();
    void declareWar(int attacker, int defender, Army& attackerArmy, Army& defenderArmy, Leader& attackerLeader, Leader& defenderLeader, Peasant& attackerPeasant, Merchant& attackerMerchant, Noble& attackerNoble, Peasant& defenderPeasant, Merchant& defenderMerchant, Noble& defenderNoble);
    void betray(int betrayer, int victim, Diplomacy& diplomacy, Army& betrayerArmy, Army& victimArmy, Leader& betrayerLeader, Leader& victimLeader, Peasant& betrayerPeasant, Merchant& betrayerMerchant, Noble& betrayerNoble, Peasant& victimPeasant, Merchant& victimMerchant, Noble& victimNoble);
    void displayWars() const;
    void saveWars() const;
    void loadWars();
private:
    struct War {
        int attacker, defender;
        bool active;
    };
    War wars[20];
    int warCount;
};

class Map {
public:
    Map();
    void movePlayer(int playerId, int newX, int newY);
    void displayMap() const;
    bool isPositionOccupied(int x, int y) const;
    void saveMap() const;
    void loadMap();
    void removePlayer(int playerId);
private:
    struct Position {
        int playerId;
        int x, y;
    };
    Position positions[10];
    int positionCount;
};

class Logger {
public:
    static void logScore(const string& info);
    static void saveGame(const Population& pop, const Army& army,
        const Bank& bank, const Leader& leader,
        const ResourceManager& rm, const Economy& economy,
        const Peasant& peasant, const Merchant& merchant, const Noble& noble);
    static bool loadGame(Population& pop, Army& army, Bank& bank,
        Leader& leader, ResourceManager& rm, Economy& economy,
        Peasant& peasant, Merchant& merchant, Noble& noble);
};

#endif