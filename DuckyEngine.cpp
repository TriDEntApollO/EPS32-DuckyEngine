#include <Arduino.h>
#include "DuckyEngine.h"

DuckyEngine::DuckyEngine(BleKeyboard &bleKeyboard) : bleKeyboard(bleKeyboard), intervalDelay(5) {
    keyMap = {
        // Basic keys
        {"ENTER", KEY_RETURN},
        {"ESC", KEY_ESC},
        {"TAB", KEY_TAB},
        {"SPACE", ' '},
        {"BACKSPACE", KEY_BACKSPACE},
        {"DELETE", KEY_DELETE},
    
        // Modifier keys
        {"CTRL", KEY_LEFT_CTRL},
        {"SHIFT", KEY_LEFT_SHIFT},
        {"ALT", KEY_LEFT_ALT},
        {"GUI", KEY_LEFT_GUI},
        {"LEFT_CTRL", KEY_LEFT_CTRL},
        {"LEFT_SHIFT", KEY_LEFT_SHIFT},
        {"LEFT_ALT", KEY_LEFT_ALT},
        {"LEFT_GUI", KEY_LEFT_GUI},
        {"RIGHT_CTRL", KEY_RIGHT_CTRL},
        {"RIGHT_SHIFT", KEY_RIGHT_SHIFT},
        {"RIGHT_ALT", KEY_RIGHT_ALT},
        {"RIGHT_GUI", KEY_RIGHT_GUI},
    
        // Navigation keys
        {"UP", KEY_UP_ARROW},
        {"DOWN", KEY_DOWN_ARROW},
        {"LEFT", KEY_LEFT_ARROW},
        {"RIGHT", KEY_RIGHT_ARROW},
        {"INSERT", KEY_INSERT},
        {"HOME", KEY_HOME},
        {"END", KEY_END},
        {"PAGE_UP", KEY_PAGE_UP},
        {"PAGE_DOWN", KEY_PAGE_DOWN},
    
        // Function keys
        {"F1", KEY_F1}, {"F2", KEY_F2}, {"F3", KEY_F3}, {"F4", KEY_F4},
        {"F5", KEY_F5}, {"F6", KEY_F6}, {"F7", KEY_F7}, {"F8", KEY_F8},
        {"F9", KEY_F9}, {"F10", KEY_F10}, {"F11", KEY_F11}, {"F12", KEY_F12},
        {"F13", KEY_F13}, {"F14", KEY_F14}, {"F15", KEY_F15}, {"F16", KEY_F16},
        {"F17", KEY_F17}, {"F18", KEY_F18}, {"F19", KEY_F19}, {"F20", KEY_F20},
        {"F21", KEY_F21}, {"F22", KEY_F22}, {"F23", KEY_F23}, {"F24", KEY_F24},
    
        // Alphanumeric cluster
        {"CAPS_LOCK", KEY_CAPS_LOCK},
        {"MENU", 0xED},
    
        // Function control cluster
        {"PRINT_SCREEN", KEY_PRTSC},
        {"SCROLL_LOCK", 0xCF},
        {"PAUSE", 0xD0},
    
        // Keypad keys
        {"NUM_LOCK", 0xDB},
        {"KP_SLASH", KEY_NUM_SLASH},
        {"KP_ASTERISK", KEY_NUM_ASTERISK},
        {"KP_MINUS", KEY_NUM_MINUS},
        {"KP_PLUS", KEY_NUM_PLUS},
        {"KP_ENTER", KEY_NUM_ENTER},
        {"KP_1", KEY_NUM_1}, {"KP_2", KEY_NUM_2}, {"KP_3", KEY_NUM_3},
        {"KP_4", KEY_NUM_4}, {"KP_5", KEY_NUM_5}, {"KP_6", KEY_NUM_6},
        {"KP_7", KEY_NUM_7}, {"KP_8", KEY_NUM_8}, {"KP_9", KEY_NUM_9},
        {"KP_0", KEY_NUM_0}, {"KP_DOT", KEY_NUM_PERIOD},
    };

    mediaKeyMap = {
        // Media keys
        {"MEDIA_PLAY", KEY_MEDIA_PLAY_PAUSE},
        {"MEDIA_PAUSE", KEY_MEDIA_PLAY_PAUSE},
        {"MEDIA_STOP", KEY_MEDIA_STOP},
        {"MEDIA_PLAY_PAUSE", KEY_MEDIA_PLAY_PAUSE},
        {"MEDIA_NEXT_TRACK", KEY_MEDIA_NEXT_TRACK},
        {"MEDIA_PREVIOUS_TRACK", KEY_MEDIA_PREVIOUS_TRACK},
        {"MEDIA_MUTE", KEY_MEDIA_MUTE},
        {"MEDIA_VOLUME_UP", KEY_MEDIA_VOLUME_UP},
        {"MEDIA_VOLUME_DOWN", KEY_MEDIA_VOLUME_DOWN},
        {"MEDIA_WWW_HOME", KEY_MEDIA_WWW_HOME},
        {"MEDIA_LOCAL_MACHINE_BROWSER", KEY_MEDIA_LOCAL_MACHINE_BROWSER},
        {"MEDIA_CALCULATOR", KEY_MEDIA_CALCULATOR},
        {"MEDIA_WWW_BOOKMARKS", KEY_MEDIA_WWW_BOOKMARKS},
        {"MEDIA_WWW_SEARCH", KEY_MEDIA_WWW_SEARCH},
        {"MEDIA_WWW_STOP", KEY_MEDIA_WWW_STOP},
        {"MEDIA_WWW_BACK", KEY_MEDIA_WWW_BACK},
        {"MEDIA_CONSUMER_CONTROL_CONFIGURATION", KEY_MEDIA_CONSUMER_CONTROL_CONFIGURATION},
        {"MEDIA_EMAIL_READER", KEY_MEDIA_EMAIL_READER}
    };
}

// Executes a given Ducky Script line by line
void DuckyEngine::executeScript(const std::string &script) {
    std::istringstream stream(script); // Convert script into a stream for reading
    std::string line;
    intervalDelay = 5; // Reset delay interval to default
    activeKeys.clear();  // Reset pressed keys before execution
    activeModifiers.clear(); // Reset active modifiers before execution
    activeMediaKeys.clear(); // Reset active media keys before execution

    std::string commandBuffer;
    
    while (std::getline(stream, line)) { // Read each line
        commandBuffer += line;
        // Process command when encountering ';' or a newline
        size_t pos;
        while ((pos = commandBuffer.find(';')) != std::string::npos) {
            std::string command = commandBuffer.substr(0, pos);
            if (!command.empty()) {
                processCommand(command);
            }
            commandBuffer.erase(0, pos + 1); // Remove processed command
        }
        // Process remaining command at end of the line
        if (!commandBuffer.empty()) {
            processCommand(commandBuffer);
            commandBuffer.clear();
        }
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
        std::string key;
        if (ss >> key) {
            sendKey(key); // Send the character as a key press
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
        if (keyMap.find(command) != keyMap.end()) {
            if (isModifierKey(keyMap[command])) {
                handleModifierCombo(command, ss); // Handle modifier combo
            } else {
                sendKey(command); // Send the key press
            }
        } else {
            // Treat the entire line (including this word) as text to print
            std::string remainder = command;
            std::string word;
            while (ss >> word) {
                remainder += " " + word;
            }
            typeString(remainder); // Character-by-character typing
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

// Handles modifier key combinations (e.g., CTRL+ALT+DEL)
void DuckyEngine::handleModifierCombo(std::string key, std::istringstream &ss) {
    std::vector<std::string> keysToRelease;
    do {
        // Press all keys till the line ends
        pressKey(key, false);
        keysToRelease.push_back(key);
    } while (ss >> key);

    // Release all keys at once
    for (std::string key : keysToRelease) {
        releaseKey(key, false);
    }
}

// Presses a key and adds it to the active key list
void DuckyEngine::pressKey(const std::string &key, bool allowLogs) {
    DuckyKey duckyKey = getKey(key);
    switch (duckyKey.type) {
        case KeyType::Standard:
            bleKeyboard.press(duckyKey.keyCode.standardKey); // Send standard key press
            break;
        case KeyType::Media:
            bleKeyboard.press(duckyKey.keyCode.mediaKey); // Send media key press
            break;
        case KeyType::Modifier:
            bleKeyboard.press(duckyKey.keyCode.modifierKey); // Send modifier key press
            break;
        default:
            break;        
    }

    // Log key presses if allowed
    if (allowLogs) {
        switch (duckyKey.type) {
            case KeyType::Standard:
                activeKeys.push_back(duckyKey.keyCode.standardKey);
                break;
            case KeyType::Media:
                activeMediaKeys.push_back(duckyKey.keyCode.mediaKey);
                break;
            case KeyType::Modifier:
                activeModifiers.push_back(duckyKey.keyCode.modifierKey);
                break;
            default:
                break;
        }
    }
}

// Releases a specific key or all keys if "ALL" is specified
void DuckyEngine::releaseKey(const std::string &key, bool allowLogs) {
    if (key == "ALL") {
        releaseAllKeys(); // Release everything
        return;
    }

    DuckyKey duckyKey = getKey(key);
    switch (duckyKey.type) {
        case KeyType::Standard:
            bleKeyboard.release(duckyKey.keyCode.standardKey); // Send standard key release
            break;
        case KeyType::Media:
            bleKeyboard.release(duckyKey.keyCode.mediaKey); // Send media key release
            break;
        case KeyType::Modifier:
            bleKeyboard.release(duckyKey.keyCode.modifierKey); // Send modifier key release
            break;
        default:
            break;        
    }

    // Log key release if allowed
    if (allowLogs) {
        switch (duckyKey.type) {
            case KeyType::Standard:
                removeKey(duckyKey.keyCode.standardKey);
                break;
            case KeyType::Media:
                removeMediaKey(duckyKey.keyCode.mediaKey);
                break;
            case KeyType::Modifier:
                removeModifier(duckyKey.keyCode.modifierKey);
                break;
            default:
                break;
        }
    }
}

// Sends a single character keystroke
void DuckyEngine::sendKey(const std::string& key) {
    DuckyKey duckyKey = getKey(key);
    switch (duckyKey.type) {
        case KeyType::Standard:
            bleKeyboard.write(duckyKey.keyCode.standardKey); // Send standard key release
            break;
        case KeyType::Media:
            bleKeyboard.write(duckyKey.keyCode.mediaKey); // Send media key release
            break;
        case KeyType::Modifier:
            bleKeyboard.write(duckyKey.keyCode.modifierKey); // Send modifier key release
            break;
        default:
            break;        
    }
}

DuckyKey DuckyEngine::getKey(const std::string& key) {
    auto keyIt = keyMap.find(key);
    if (keyIt != keyMap.end()) {
        return DuckyKey(keyIt->second, isModifierKey(keyIt->second));
    }

    auto mediaIt = mediaKeyMap.find(key);
    if (mediaIt != mediaKeyMap.end()) {
        return DuckyKey(mediaIt->second);
    }

    // Fallback: Return first character of the string as a normal key
    return DuckyKey(key.empty() ? 0 : key[0]);
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
    return (
        keyCode == KEY_LEFT_CTRL  || keyCode == KEY_RIGHT_CTRL  ||
        keyCode == KEY_LEFT_SHIFT || keyCode == KEY_RIGHT_SHIFT ||
        keyCode == KEY_LEFT_ALT   || keyCode == KEY_RIGHT_ALT   ||
        keyCode == KEY_LEFT_GUI   || keyCode == KEY_RIGHT_GUI
    );
}

// Removes a non-modifier key from activeKeys
void DuckyEngine::removeKey(uint8_t keyCode) {
    activeKeys.erase(std::remove(activeKeys.begin(), activeKeys.end(), keyCode), activeKeys.end());
}

// Removes a media key from activeMediaKeys
void DuckyEngine::removeMediaKey(const uint8_t* keyCode) {
    activeMediaKeys.erase(std::remove(activeMediaKeys.begin(), activeMediaKeys.end(), keyCode), activeMediaKeys.end());
}

// Removes a modifier key from activeModifiers
void DuckyEngine::removeModifier(uint8_t keyCode) {
    activeModifiers.erase(std::remove(activeModifiers.begin(), activeModifiers.end(), keyCode), activeModifiers.end());
}

// Releases all currently active keys 
void DuckyEngine::releaseAllKeys() {
    for (uint8_t keyCode : activeKeys) {
        bleKeyboard.release(keyCode);
    }
    for (const uint8_t* keyCode : activeMediaKeys) {
        bleKeyboard.release(keyCode);
    }
    for (uint8_t keyCode : activeModifiers) {
        bleKeyboard.release(keyCode);
    }

    activeKeys.clear();
    activeMediaKeys.clear();
    activeModifiers.clear();
}
