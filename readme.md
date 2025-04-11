# 💣 Minesweeper Circuit — Hardware Implementation with Serial Communication

An interactive hardware-based version of the classic **Minesweeper game**, implemented using **combinational and sequential logic circuits**, a **custom RAM**, and controlled via **serial communication** using an **Arduino and Bluetooth**.

Developed as a capstone project for the **Organización Computacional** course at **Universidad de San Carlos de Guatemala**.

---

## 🧠 Core Concepts

- 🧮 **Combinational & Sequential Circuits**
- 📡 **Serial Communication (USB & Bluetooth)**
- 🖥️ **Frontend + Backend Architecture**
- 🔄 **Finite State Machine (FSM) for game logic**
- 🕹️ **Physical game interface using LEDs and buttons**

---

## 🎯 Objectives

### ✅ General
Apply all core concepts from the course to build an integrated digital system.

### ✅ Specific
- Design and implement **custom RAM**
- Apply **serial communication protocols**
- Integrate **sequential logic** to manage game states
- Create a functional **hardware-software** game prototype

---

## 🎮 Project Overview

This Minesweeper version runs on a physical board and is playable through:

- A **frontend UI or `.org` configuration file** for bomb placement
- **Bluetooth terminal** for gameplay via smartphone

### Game States:
| State     | Description                           | LED  |
|-----------|---------------------------------------|------|
| Playing   | Game in progress                      | 🔵 Blue |
| Game Over | Stepped on a mine                     | 🔴 Red |
| Victory   | All safe positions uncovered          | 🟢 Green |

---

## 🧩 System Architecture

```
Frontend (Web with Vue)
    ↓
Backend (Bomb coordinates, game logic with Flask)
    ↓
Serial Communication (via Arduino + HC-06 Bluetooth)
    ↓
Hardware Circuit:
    - Game Logic (FSM)
    - LCD Display (Score & Status)
    - LED Indicators
```

---

## 🛠️ Technologies & Components

| Component     | Description                         |
|---------------|-------------------------------------|
| HC-06         | Bluetooth Module                    |
| LCD 16x2      | Score and state display             |
| Arduino       | Serial interface + Bluetooth control|

---

## 📦 Repository Structure & Workflow

Git branching strategy to ensure collaboration:

- `main`: Stable releases only
- `develop`: Integrates all features
- `feature/<funcionalidad>_<carnet>`: Individual feature branches

---

## 📝 How to Play

### 1. Configure the Board
- Use the frontend UI or upload a `.org` file with coordinates.
- RAM (4x4) stores bomb locations and lights up LEDs accordingly.

### 2. Start the Game
- Via Bluetooth terminal:
  - Send a number (1–16) to check a position.
  - Receive "acierto correcto" or "game over".
- LCD displays the score and game status.

### 3. Reset
Send `"reinicio"` via terminal to restart the game.

---

## 📸 Screenshots (TODO: ADD PICTURES)

---

## 📩 Contact

Feel free to reach out if you want to collaborate, learn more about digital systems, or check out the repo:

📧 marcosbonifasi19@gmail.com  
💼 [LinkedIn](https://www.linkedin.com/in/marcosbondel/)
