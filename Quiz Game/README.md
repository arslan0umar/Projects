# Quiz Master OOP Edition 🧠🎮

![Quiz Master](https://img.shields.io/badge/Quiz_Master_C%2B%2B-blue.svg)

## Link

[📺 Watch Gameplay Demo (Google Drive)](https://drive.google.com/file/d/1QC2S3mVD7L1NCnXgoJjv9rJOHVF2Rl3N/view?usp=sharing) <!-- Replace with actual link -->

## Table of Contents

* [Description](#description)
* [New Features](#new-features)
* [Features](#features)
* [Installation](#installation)
* [File Structure](#file-structure)
* [Controls](#controls)
* [Game Mechanics](#game-mechanics)
* [Persistence](#persistence)
* [Development](#development)

---

## Description

**Quiz Master** is a C++ console-based quiz game offering an engaging educational experience through multiple-choice questions. With two gameplay modes and three difficulty levels, players test their knowledge in real-time with score tracking and leaderboard systems.

✨ Highlights:
* Supports single-player and two-player modes
* Difficulty levels: Easy, Medium, and Hard
* Multiple dynamic question files
* Live scoring system with penalties
* Clean console UI with timer

---

## New Features

### Leaderboard & Score Tracking

* Real-time score calculation
* Automatic ranking system
* High scores saved in `highscores.txt`

### Dual Mode Support

* One-player and two-player modes
* Turn-based answering
* Separate scoreboards

### Dynamic Question Bank

* Loads questions from `.txt` files dynamically
* Supports custom question sets

---

## Features

### Game Modes

- **Single Player**: Answer timed questions and set high scores.
- **Two Player**: Turn-based battle of wits.

### Difficulty Levels

- Easy  
- Medium  
- Hard  

Each level loads different question sets to challenge players appropriately.

---

## Installation

1. Clone or download the repository:

```bash
git clone https://github.com/yourusername/QuizMaster-OOP.git
```

2. Make sure the following files are in the same directory:
   - All `questions*.txt` files
   - `highscores.txt`

3. Compile the program using g++:

```bash
g++ main.cpp -o quizgame
```

4. Run the game:

```bash
./quizgame
```

> 📦 No external libraries needed. Only standard C++ headers are used.

---

## File Structure

```
main.cpp                    # Main game logic
questionseasy.txt           # Easy level questions
questionsmedium.txt         # Medium level questions
questionshard.txt           # Hard level questions
newquestionseasy.txt        # Alternative question bank
newquestionsmedium.txt
newquestionshard.txt
highscores.txt              # Stores leaderboard
Project Description.docx    # Design and documentation
```

---

## Controls

| Input         | Action                          |
| ------------- | ------------------------------- |
| 1 / 2         | Select game mode (1P or 2P)     |
| 1/2/3/4       | Answer question (MCQs)          |
| ENTER         | Proceed to next question/menu   |
| ESC           | Exit the game                   |

---

## Game Mechanics

### Timer

- Each question has a time limit of 10 seconds.
- Late answers may result in a score penalty.

### Scoring

- Correct: +10 points  
- Wrong:  0 points  
- Skipped: -5 points (in some modes)

### Leaderboard

- Maintained in `highscores.txt`
- Top scores recorded per mode and difficulty

---

## Persistence

Saved to `highscores.txt`:

- Player names
- Scores
- Difficulty levels
- Game mode

---

## Development

### Built With:

* C++ (Standard Libraries)
* Header files for modular structure

---

🛠️ **This project is open to collaboration. Fork it, customize questions, improve UI, or add new features. Contributions are welcome!** 🚀
