#include "DuckyEngine.h"

#include <Arduino.h>

DuckyEngine::DuckyEngine(BleKeyboard &bleKeyboard) : bleKeyboard(bleKeyboard), intervalDelay(5) {
    keyMap = {
        {"ENTER", KEY_RETURN},
        {"CTRL", KEY_LEFT_CTRL},
        {"ALT", KEY_LEFT_ALT},
        {"SHIFT", KEY_LEFT_SHIFT},
        {"GUI", KEY_LEFT_GUI},
        {"ESC", KEY_ESC},
        {"TAB", KEY_TAB},
        {"SPACE", ' '},
        {"BACKSPACE", KEY_BACKSPACE},
        {"DELETE", KEY_DELETE},
        {"UP", KEY_UP_ARROW},
        {"DOWN", KEY_DOWN_ARROW},
        {"LEFT", KEY_LEFT_ARROW},
        {"RIGHT", KEY_RIGHT_ARROW},
    };
}

// Executes a given Ducky Script line by line
void DuckyEngine::executeScript(const std::string &script) {
    std::istringstream stream(script); // Convert script into a stream for reading line by line
    std::string line;
    activeKeys.clear();  // Reset pressed keys before execution
    activeModifiers.clear(); // Reset active modifiers before execution

    while (std::getline(stream, line)) { // Read each line
        processCommand(line); // Process and execute the command
    }
    releaseAllKeys(); // Ensure all keys are released at the end
}

// Processes an individual command line from the script
void DuckyEngine::processCommand(const std::string &line) {
    if (line.empty()) return; // Ignore empty lines

    std::istringstream ss(line);
    std::string command;
    ss >> command; // Extract the command name (first word)

    // Ignore the entire comment line
    if (command == "REM" || command == "#") {
        return; 
    } 
    // Handle STRING, ECHO, and PRINT commands (text output)
    else if (command == "STRING" || command == "ECHO" || command == "PRINT") {
        std::string text = extractString(ss);  // Extract the text argument
        if (text.empty()) 
            return;

        if (command == "STRING" || command == "ECHO") {
            typeString(text);
        } else {
            typePrint(text);
        }
    } 
    // Sends a single character keystroke
    else if (command == "SEND") {
        char c;
        if (ss >> c) {
            bleKeyboard.write(c);
        }
    } 
    // Handles delay (can be absolute or modify global delay interval)
    else if (command == "DELAY") {
        std::string arg;
        ss >> arg;
        if (arg == "INTERVAL") {
            ss >> intervalDelay; // Update global delay interval
        } else {
            delayExecution(std::stoi(arg));
        }
    } 
    // Handles key press
    else if (command == "PRESS") {
        std::string key;
        ss >> key;
        pressKey(key, true);
    } 
    // Handles key release (specific key or all keys)
    else if (command == "RELEASE") {
        std::string key;
        if (ss >> key) {
            releaseKey(key, true);
        } else {
            releaseAllKeys();
        }
    } 
    // Handles modifier key combinations and normal key presses
    else {
        std::vector<std::string> keysToRelease;
        bool isModifierLine = false;

        if (keyMap.find(command) != keyMap.end()) {
            isModifierLine = isModifierKey(keyMap[command]);
        }
                
        if (isModifierLine) {
            do {
                // Press all keys till the line ends
                pressKey(command, false);
                keysToRelease.push_back(command);
            } while (ss >> command);

            // Release all keys at once
            for (std::string key : keysToRelease) {
                releaseKey(key, false);
            }
        } else {
            pressKey(command, false);
            releaseKey(command, false);
        }
    }
}

// Extracts a properly quoted string or a regular space-separated argument
std::string DuckyEngine::extractString(std::istringstream &ss) {
    std::string text;
    ss >> std::ws; // Trim leading spaces
    char quoteChar = ss.peek();
    
    if (quoteChar == '"' || quoteChar == '\'') { // Detect opening quote
        ss.get(); // Consume opening quote
        std::getline(ss, text, quoteChar); // Read until closing quote
        
        // Handle escape sequences
        size_t pos = 0;
        while ((pos = text.find("\\", pos)) != std::string::npos) {
            if (pos + 1 < text.size()) {
                char nextChar = text[pos + 1];
                if (nextChar == '"' || nextChar == '\'') {
                    text.erase(pos, 1); // Remove escape character
                }
            }
            pos++;
        }
    } else {
        std::getline(ss, text); // Read entire line as a normal string
    }
    return text;
}

// Presses a key and adds it to the active key list
void DuckyEngine::pressKey(const std::string &key, bool allowModifiers) {
    uint8_t keyCode;
        if (keyMap.find(key) != keyMap.end()) {
            keyCode = keyMap[key];
        } else {
            keyCode = key[0];
        }

        // Store modifiers separately from regular keys
        if (allowModifiers && isModifierKey(keyCode)) {
            activeModifiers.push_back(keyCode);
        } else {
            activeKeys.push_back(keyCode);
        }

        bleKeyboard.press(keyCode); // Send key press
}

// Releases a specific key or all keys if "ALL" is specified
void DuckyEngine::releaseKey(const std::string &key, bool allowModifiers) {
    if (key == "ALL") {
        releaseAllKeys(); // Release everything
        return;
    }

    uint8_t keyCode;
    if (keyMap.find(key) != keyMap.end()) {
        keyCode = keyMap[key];
    } else {
        keyCode = key[0];
    }

    // Remove key from active lists
    if (allowModifiers && isModifierKey(keyCode)) {
        removeModifier(keyCode);
    } else {
        removeKey(keyCode);
    }

    bleKeyboard.release(keyCode); // Send key release
}

// Types a string instantly
void DuckyEngine::typeString(const std::string &text) {
    bleKeyboard.print(text.c_str());
}

// Types a string with per-character delay (PRINT)
void DuckyEngine::typePrint(const std::string &text) {
    for (char c : text) {
        bleKeyboard.write(c);
        delay(intervalDelay);
    }
}

// Introduces a delay
void DuckyEngine::delayExecution(int milliseconds) {
    delay(milliseconds);
}

// Checks if a key is a modifier
bool DuckyEngine::isModifierKey(uint8_t keyCode) {
    return (keyCode == KEY_LEFT_CTRL || keyCode == KEY_LEFT_ALT || keyCode == KEY_LEFT_SHIFT || keyCode == KEY_LEFT_GUI);
}

// Removes a modifier key from activeModifiers
void DuckyEngine::removeModifier(uint8_t keyCode) {
    activeModifiers.erase(std::remove(activeModifiers.begin(), activeModifiers.end(), keyCode), activeModifiers.end());
}

// Removes a non-modifier key from activeKeys
void DuckyEngine::removeKey(uint8_t keyCode) {
    activeKeys.erase(std::remove(activeKeys.begin(), activeKeys.end(), keyCode), activeKeys.end());
}

// Releases all currently active keys 
void DuckyEngine::releaseAllKeys() {
    for (uint8_t keyCode : activeModifiers) {
        bleKeyboard.release(keyCode);
    }
    for (uint8_t keyCode : activeKeys) {
        bleKeyboard.release(keyCode);
    }
    activeKeys.clear();
    activeModifiers.clear();
}
