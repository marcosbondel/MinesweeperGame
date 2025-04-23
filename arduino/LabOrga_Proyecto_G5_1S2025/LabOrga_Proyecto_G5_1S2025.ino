#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

// LCD Configuration
LiquidCrystal_I2C lcd(0x27, 16, 2);

// LED Matrix Pins
const int columnas[8] = {40, 42, 44, 46};
const int filas[4] = {48, 50, 52, 53};

// LEDS for manageing statuses: blue, green and red
const int blueLEDPin = 4;
const int greenLEDPin = 3;
const int redLEDPin = 2;

// Keypad Pins
const int filasKeypad[4] = {22, 24, 26, 28};
const int columnasKeypad[4] = {30, 32, 34, 36};

int currentRow = 0;
int currentColumn = 0;

// Game Data Structures
struct Bomb {
  int x;
  int y;
};

// EEPROM Configuration
const int MAX_BOMBS = 16;
const int MAX_POINTS = 16;

// Revised EEPROM Addresses for better organization
const int ADDRESS_COUNT_BOMBS = 0;
const int ADDRESS_BOMBS = 2;          // Starts after count (2 bytes)
const int ADDRESS_COUNT_POINTS = 100; // Separate section for points
const int ADDRESS_POINTS = 102;       // Starts after points count

// Game State Variables
int current_points_counter = 0;
String game_status = "";
String concatenatedCoord = "";

// Define a structure for coordinates
struct Coordinate {
  int x;
  int y;
};

// Dictionary entry structure
struct DictEntry {
  String key;
  Coordinate value;
};

// Dictionary implementation
class CoordinateDictionary {
private:
  static const int MAX_SIZE = 20; // Maximum entries
  DictEntry entries[MAX_SIZE];
  int size = 0;

public:
  // Add or update an entry
  void set(String key, Coordinate coord) {
    // Check if key exists
    for (int i = 0; i < size; i++) {
      if (entries[i].key == key) {
        entries[i].value = coord;
        return;
      }
    }
    
    // Add new entry if space available
    if (size < MAX_SIZE) {
      entries[size].key = key;
      entries[size].value = coord;
      size++;
    }
  }

  // Get a value by key
  bool get(String key, Coordinate &result) {
    for (int i = 0; i < size; i++) {
      if (entries[i].key == key) {
        result = entries[i].value;
        return true;
      }
    }
    return false; // Key not found
  }

  // Remove an entry
    bool remove(String key) {
        for (int i = 0; i < size; i++) {
        if (entries[i].key == key) {
            // Shift remaining entries
            for (int j = i; j < size-1; j++) {
                entries[j] = entries[j+1];
                }
                size--;
            return true;
            }
        }
        return false;
    }

    // Get current size
    int getSize() {
        return size;
    }
};

CoordinateDictionary coordDict;
Coordinate result;
String msgBluetooth = "";
String game_mode = "";


// Bomb Functions
void saveBomb(Bomb bomb) {
    int count = readBombsCount();
    if (count >= MAX_BOMBS) {
        Serial.println("Cannot save more bombs");
        return;
    }

    int address = ADDRESS_BOMBS + count * sizeof(Bomb);
    EEPROM.put(address, bomb);
    saveBombsCount(count + 1);
}

Bomb readBomb(int index) {
    Bomb p;
    int address = ADDRESS_BOMBS + index * sizeof(Bomb);
    EEPROM.get(address, p);
    return p;
}

void saveBombsCount(int count) {
    EEPROM.put(ADDRESS_COUNT_BOMBS, count);
}

int readBombsCount() {
    int count;
    EEPROM.get(ADDRESS_COUNT_BOMBS, count);
    return count;
}

// Points Functions
void savePoint(int value) {
    int count = readPointsHistory();
    if (count >= MAX_POINTS) {
        // Implement FIFO if we reach max points
        for (int i = 1; i < MAX_POINTS; i++) {
            int val = readPoint(i);
            EEPROM.put(ADDRESS_POINTS + (i-1)*sizeof(int), val);
        }
        count = MAX_POINTS - 1;
    }
    
    EEPROM.put(ADDRESS_POINTS + count * sizeof(int), value);
    savePointsCount(count + 1);
    
    Serial.print("Saved points: ");
    Serial.println(value);
}

int readPoint(int index) {
    int value;
    int address = ADDRESS_POINTS + index * sizeof(int);
    EEPROM.get(address, value);
    return value;
}

void savePointsCount(int count) {
    EEPROM.put(ADDRESS_COUNT_POINTS, count);
}

int readPointsHistory() {
    int count;
    EEPROM.get(ADDRESS_COUNT_POINTS, count);
    // Validate count is within bounds
    if (count < 0 || count > MAX_POINTS) {
        return 0;
    }
    return count;
}

// Game Management Functions
void deleteBombs() {
    saveBombsCount(0);
    Serial.println("All bombs deleted");
}

// Verifies if the coordinates exists
bool objectExists(int x_val, int y_val) {
    int count = readBombsCount();
    for (int i = 0; i < count; i++) {
        Bomb bomb = readBomb(i);
        if (bomb.x == x_val && bomb.y == y_val) {
            return true;
        }
    }
    return false;
}

void displayPointsHistory() {
    int count = readPointsHistory();
    Serial.print("Points History (");
    Serial.print(count);
    Serial.println(" entries):");
    
    for (int i = 0; i < count; i++) {
        Serial.print(i);
        Serial.print(": ");
        Serial.println(readPoint(i));
    }
}

// Clean EEPROM
// Frees the memory 
void clean_eeprom() {
    Serial.println("Clearing EEPROM...");
    for (int i = 0; i < EEPROM.length(); i++) {
        EEPROM.write(i, 0xFF);
    }
    #if !defined(ARDUINO_ARCH_AVR)
        EEPROM.commit();
    #endif
    Serial.println("EEPROM cleared!");
    saveBombsCount(0);
    savePointsCount(0);
}

// Setup and Main Loop
void setup() {
    Serial.begin(9600);
    Serial1.begin(9600);  // Serial1 (Only for mega): HC-06 (Pines 18-TX1, 19-RX1)

    // LEDs matrix
    for (int i = 0; i < 4; i++) {
        pinMode(filas[i], OUTPUT);
        pinMode(columnas[i], OUTPUT);
        digitalWrite(filas[i], HIGH); // Turn off columns
        digitalWrite(columnas[i], LOW); // Turn off rows
    }
    
    // LEDs of statuses
    pinMode(blueLEDPin, OUTPUT);
    pinMode(greenLEDPin, OUTPUT);
    pinMode(redLEDPin, OUTPUT);
    digitalWrite(blueLEDPin, LOW);
    digitalWrite(greenLEDPin, LOW);
    digitalWrite(redLEDPin, LOW);
    
    // LCD Initialization
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Grupo 5 - Orga");
    lcd.setCursor(0, 1);
    lcd.print("MINESWEEPER");

    // Keypad Initialization
    for (int i = 0; i < 4; i++) {
        pinMode(filasKeypad[i], OUTPUT);
        digitalWrite(filasKeypad[i], HIGH);
        pinMode(columnasKeypad[i], INPUT_PULLUP);
    }

    // Initialize game state
    current_points_counter = 0;
    game_status = "";
    
    // Uncomment only when you need to clear EEPROM
    clean_eeprom();
    
    // Initialize bomb count if invalid
    if (readBombsCount() < 0 || readBombsCount() > MAX_BOMBS) {
        saveBombsCount(0);
    }
    
    // Initialize points count if invalid
    if (readPointsHistory() < 0 || readPointsHistory() > MAX_POINTS) {
        savePointsCount(0);
    }

    // Register keypad, values for bombs coordinates
    coordDict.set("11", {1,1});
    coordDict.set("12", {1,2});
    coordDict.set("13", {1,3});
    coordDict.set("14", {1,4});
    coordDict.set("21", {2,1});
    coordDict.set("22", {2,2});
    coordDict.set("23", {2,3});
    coordDict.set("24", {2,4});
    coordDict.set("31", {3,1});
    coordDict.set("32", {3,2});
    coordDict.set("33", {3,3});
    coordDict.set("34", {3,4});
    coordDict.set("41", {4,1});
    coordDict.set("42", {4,2});
    coordDict.set("43", {4,3});
    coordDict.set("44", {4,4});

    Serial.println("Arduino ready!");
    displayPointsHistory();
}

void loop() {
    // Call keypad and bluetooth method only if the status is 'playing'
    if(game_status == "playing"){
        keypad();
        inputBluetooth();
    }

    // Serial Listener
    // Reads the input from the serial connections (backend)
    if (Serial.available() > 0) {
        String incoming = Serial.readStringUntil('\n');
        incoming.trim();
        
        String request_backend[3];
        splitString(incoming, " ", request_backend, 3);

        if (request_backend[0] == "add_bomb") {
            Serial.print("AddBomb x: ");
            Serial.print(request_backend[1]);
            Serial.print(" y: ");
            Serial.println(request_backend[2]);
            Bomb new_bomb = {request_backend[1].toInt(), request_backend[2].toInt()};
            saveBomb(new_bomb);
        } else if (request_backend[0] == "reset_game") {
            savePoint(current_points_counter);
            current_points_counter = 0;
            deleteBombs();
            game_status = "";
        } else if (request_backend[0] == "increment_points") {
            increment_points();
        } else if (request_backend[0] == "verify_bomb") {
            Serial.print("verify_bomb x: ");
            Serial.print(request_backend[1]);
            Serial.print(" y: ");
            Serial.println(request_backend[2]);

            verify_bomb(request_backend[1].toInt(), request_backend[2].toInt());
        } else if (request_backend[0] == "play_game") {

            if(game_status == "playing"){
                return;
            }

            game_mode = request_backend[1];

            game_status = "playing";
            Serial.println("Game started");
            
            // Show Message in LCD
            lcd.clear();
            lcd.print("PLAYING ...");
            delay(3000);
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Points: ");
            lcd.setCursor(0, 1);
            lcd.print(current_points_counter);
            digitalWrite(blueLEDPin, HIGH);
            
        } else if (request_backend[0] == "show_points") {
            displayPointsHistory();
        } else if (request_backend[0] == "won"){
            won();
        } else if(request_backend[0] == "show_top5"){
            displayTop5Scores();
        }
    }
}

// Utility Functions
void splitString(String input, String delimiter, String results[], int maxCount) {
    int count = 0;
    int index = 0;
    int delimiterLength = delimiter.length();
    
    while (count < maxCount - 1) {
        int nextIndex = input.indexOf(delimiter, index);
        
        if (nextIndex == -1) {
            results[count++] = input.substring(index);
            break;
        }
        
        results[count++] = input.substring(index, nextIndex);
        index = nextIndex + delimiterLength;
    }
    
    if (count < maxCount && index < input.length()) {
        results[count] = input.substring(index);
    }
}

// KEYPAD
// Code for scanning when a button is pressed, some actions should be performed
void keypad() {
    if(game_mode != "Keypad"){
        return;
    }

    static bool buttonPressed = false; // Track button state

    for (int f = 0; f < 4; f++) {
        digitalWrite(filasKeypad[f], LOW); // Activate one row at a time

        for (int c = 0; c < 4; c++) {
            if (digitalRead(columnasKeypad[c]) == LOW) {
                if (!buttonPressed) { // Only process if this is a new press
                    buttonPressed = true;
                    
                    currentRow = f + 1;
                    currentColumn = c + 1;
                    
                    Serial.print("Tecla presionada en Fila ");
                    Serial.print(currentRow);
                    Serial.print(", Columna ");
                    Serial.println(currentColumn);

                    concatenatedCoord = String(currentRow) + String(currentColumn);

                    coordDict.get(concatenatedCoord, result);
                    
                    if (!verify_bomb(result.x, result.y)) {
                        increment_points();
                    }

                    if (won()) {
                        digitalWrite(filasKeypad[f], HIGH); // Deactivate row before breaking
                        return;
                    }
                }
                
                // Wait until button is released with proper debouncing
                while (digitalRead(columnasKeypad[c]) == LOW) {
                    delay(10);
                }
                buttonPressed = false; // Reset for next press
                delay(50); // Additional debounce delay
            }
        }

        digitalWrite(filasKeypad[f], HIGH); // Deactivate row
        delay(1); // Small delay between rows
    }
}

// VERIFY BOMB
// Check if there's an existing bomb in the given coordinates
bool verify_bomb(int x, int y){
    bool gameover = objectExists(x, y);
            
    if (gameover) {
        // Turn on led for 3 seconds
        turnOnLEDBomb(x,y);

        savePoint(current_points_counter);
        current_points_counter = 0;
        deleteBombs();
        game_status = "game_over";
        Serial.println("Game over!");
        
        // Show msg in LCD 
        lcd.clear();
        lcd.print("GAMEOVER ...");
        digitalWrite(blueLEDPin, LOW);
        digitalWrite(redLEDPin, HIGH);
        delay(3000);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Grupo 5 - Orga");
        lcd.setCursor(0, 1);
        lcd.print("MINESWEEPER");
        digitalWrite(redLEDPin, LOW);
        game_mode = "";

        return true;
    }

    Serial.println("GREEN - Bombs free!");
    Serial.print("x: ");
    Serial.println(x);
    Serial.print("y: ");
    Serial.println(y);

    return false;
}

// INCREMENT POINTS
// Increments the points of the current game
void increment_points(){
    current_points_counter++;
    Serial.print("Current points: ");
    Serial.println(current_points_counter);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Points: ");
    lcd.setCursor(0, 1);
    lcd.print(current_points_counter);
}

// TURN ON RED - MATRIX OF LED
// Turns on the LED of the given coordinates
void turnOnLEDBomb(int x, int y){
    // Turn on LED
    digitalWrite(filas[x-1], LOW);
    digitalWrite(columnas[y-1], HIGH);
    
    // Wait 3 seconds
    delay(3000);

    // Turn off LED
    digitalWrite(filas[x-1], HIGH);
    digitalWrite(columnas[y-1], LOW);
}

// BLUETOOTH
// Serial communication between Arduino and Phone(Bluetooth) via serial
void inputBluetooth(){
    if(game_mode != "Bluetooth"){
        return;
    }

    if (Serial1.available()) {
        char data = Serial1.read();
        msgBluetooth = msgBluetooth + data;

        if(msgBluetooth.length() == 3){
            Serial1.print("Should check coordinates: ");
            Serial1.println(msgBluetooth);

            String coordBluetooth[2];
            splitString(msgBluetooth, ",", coordBluetooth, 2);
            if(!verify_bomb(coordBluetooth[0].toInt(), coordBluetooth[1].toInt())) {
                increment_points();
            }
            won();
            msgBluetooth = "";
        }   
    }
}

// WON
// Checks if the user has won
bool won(){

    if(current_points_counter == (16 - readBombsCount())){
        savePoint(current_points_counter);
        current_points_counter = 0;
        deleteBombs();
        Serial.println("Game over! Points saved.");
        lcd.clear();
        lcd.print("YOU'VE WON :)");
        digitalWrite(blueLEDPin, LOW);
        digitalWrite(greenLEDPin, HIGH);
        delay(3000);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Grupo 5 - Orga");
        lcd.setCursor(0, 1);
        lcd.print("MINESWEEPER");
        game_status = "won";
        digitalWrite(greenLEDPin, LOW);
        game_mode = "";
        return true;
    }

    return false;
}

int* getTop5Scores() {
    static int topScores[5] = {0, 0, 0, 0, 0}; // Initialize all to 0
    int count = readPointsHistory();
    
    // If no scores exist, return all zeros
    if (count == 0) {
        return topScores;
    }
    
    // Read all scores into a temporary array
    int scores[MAX_POINTS];
    for (int i = 0; i < count; i++) {
        scores[i] = readPoint(i);
    }
    
    // Sort the scores in descending order (using simple bubble sort)
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (scores[j] < scores[j+1]) {
                // Swap scores
                int temp = scores[j];
                scores[j] = scores[j+1];
                scores[j+1] = temp;
            }
        }
    }
    
    // Copy the top 5 (or fewer if count < 5)
    int numToCopy = (count < 5) ? count : 5;
    for (int i = 0; i < numToCopy; i++) {
        topScores[i] = scores[i];
    }
    
    return topScores;
}

void displayTop5Scores() {
    int* topScores = getTop5Scores();
    String topScoresStr = "";
    
    Serial.println("Top 5 Scores:");
    for (int i = 0; i < 5; i++) {
        Serial.print(i+1);
        Serial.print(": ");
        Serial.println(topScores[i]);
        if(i == 4){
            topScoresStr = topScoresStr + topScores[i];
        }else{
            topScoresStr = topScoresStr + topScores[i] + ", ";
        }
    }

    // Show Top5 in LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Top 5 - Points");
    lcd.setCursor(0, 1);
    lcd.print(topScoresStr);
    delay(5000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Grupo 5 - Orga");
    lcd.setCursor(0, 1);
    lcd.print("MINESWEEPER");
}


