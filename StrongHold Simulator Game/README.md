# StrongHold Simulator Game 🏰⚔️

![StrongHold](https://img.shields.io/badge/StrongHold_C%2B%2B-red.svg)

## Link

[📺 Watch Gameplay Demo (Google Drive)](https://drive.google.com/file/d/1Phb31om_TIofXLV91pWrW_ad3MDJoD-9/view?usp=drive_link) <!-- Replace with actual link -->

## Table of Contents

* [Description](#description)
* [Features](#features)
* [Installation](#installation)
* [File Structure](#file-structure)
* [Controls](#controls)
* [Game Mechanics](#game-mechanics)
* [Persistence](#persistence)
* [Development](#development)

---

## Description

**StrongHold Simulator Game** is a C++ console-based turn-based strategy game where players build kingdoms, manage resources, lead armies, and engage in diplomacy or warfare. The game combines economy, military tactics, and leadership elements, providing a rich simulation of medieval strategy.

✨ Highlights:
* Multi-player turn-based gameplay
* Kingdom management (economy, army, diplomacy)
* War simulation with morale and popularity effects
* Dynamic map, events, and trade system
* Save and load functionality

---



## Features

### Core Systems

- **Army Management**: Train and maintain soldiers.
- **Economy**: Manage resources, taxes, and trade.
- **Diplomacy**: Form alliances, trade agreements, or declare wars.
- **Events**: Random events influence gameplay.

### Victory Conditions

- Eliminate other kingdoms through war or diplomacy.
- Maintain stability and growth of your kingdom.

### War Outcome System

* War results directly affect morale, leader popularity, and satisfaction.
* Victorious kingdoms gain reputation, while defeated kingdoms lose morale.

### Kingdom Elimination

* If a kingdom's population reaches zero, it is eliminated and removed from the map.
* Victory is declared when only one kingdom remains.

### Dynamic Market & Trade

* Market transactions recorded in `market_log.txt`.
* Prices and resources dynamically updated.

### Population & Events 🏰  
The game introduces a dynamic population system with **random events** that can positively or negatively impact the kingdom. Citizens are born over time, contributing to natural **population growth**, while **migration** brings in new people seeking better opportunities. At the same time, players must manage challenges like **deaths** caused by old age, disease, starvation, or unexpected events. These mechanics create a realistic cycle of growth and decline, making strategic resource management and decision-making essential for survival.

---

## Installation

1. Clone or download the repository:

```bash
git clone https://github.com/yourusername/StrongHold-Simulator.git
```

2. Make sure the following files are in the same directory:
   - `map.txt`
   - `market.txt`
   - `game_save.txt`

3. Compile the program using g++:

```bash
g++ main.cpp Army.cpp Bank.cpp Communication.cpp Conflict.cpp Diplomacy.cpp Economy.cpp EventManager.cpp Leader.cpp Logger.cpp Map.cpp Market.cpp Merchant.cpp Noble.cpp Religion.cpp Kingdom.cpp -o stronghold
```

4. Run the game:

```bash
./stronghold
```

> 📦 No external libraries needed. Only standard C++ headers are used.

---

## File Structure

```
main.cpp                    # Main game driver
Army.cpp                    # Army management system
Bank.cpp                    # Economy & finance
Communication.cpp           # Messaging system
Conflict.cpp                # War simulation mechanics
Diplomacy.cpp               # Diplomacy between kingdoms
Economy.cpp                 # Resource and trade system
EventManager.cpp            # Random events
Leader.cpp                  # Leader stats and popularity
Logger.cpp                  # Logging system
Map.cpp                     # Map rendering & management
Market.cpp                  # Market system
Merchant.cpp                # Merchant operations
Noble.cpp                   # Noble class and hierarchy
Religion.cpp                # Religion system
Kingdom.cpp                 # Core Kingdom class
map.txt                     # Base map file
map_save.txt                # Saved map state
market.txt                  # Market configuration
market_log.txt              # Market transactions
messages.txt                # In-game communication logs
game_save.txt               # Save file
Project Description.docx    # Design and documentation
```

---

## Controls

| Input             | Action                                |
| ----------------- | ------------------------------------- |
| 1 / 2 /...        | Select actions (Attack, Trade, etc.)  |
| ENTER             | Proceed to next turn/menu             |
| ESC               | Exit the game                         |

---

## Game Mechanics

### Turn-Based Gameplay

- Two players take turns managing their kingdoms.
- Each turn allows military, economic, or diplomatic actions.

### War System

- Victory boosts morale, popularity, and satisfaction.
- Defeat lowers soldier morale and leader reputation.

### Scoring / Victory

- Survival until the end determines the winner.
- Kingdoms with zero population are removed.

---

## Persistence

Saved to files:

- `game_save.txt`: Stores overall game progress
- `map_save.txt`: Saves the map state
- `market_log.txt`: Records trade history
- `highscores.txt` (optional): Can track victories

---

## Development

### Built With:

* C++ (Standard Libraries)
* Modular OOP structure with multiple classes

---

🛠️ **This project is open to collaboration. Fork it, improve mechanics, add AI opponents, or expand kingdom features. Contributions are welcome!** 🚀
