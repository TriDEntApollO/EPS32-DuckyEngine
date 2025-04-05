/*
  DuckyEngine.h - Library for executing Ducky Script commands over BLE.
  Created by TridentApollo, 2nd April, 2025.
  Designed for use with the ESP32 and the BleKeyboard library.
  This library allows an ESP32 to simulate a Bluetooth keyboard and execute scripted keystrokes.

  Released into the public domain.
*/

#ifndef DUCKY_ENGINE_H
#define DUCKY_ENGINE_H

#include <map>
#include <vector>
#include <variant>
#include <sstream>
#include <BleKeyboard.h>

enum class KeyType {
    Standard,
    Media,
    Modifier
};

struct DuckyKey {
    KeyType type;
    union {
        uint8_t standardKey;        // e.g., 'A', '1', KEY_LEFT_SHIFT, etc.
        uint8_t modifierKey;        // e.g., KEY_LEFT_SHIFT, GUI, etc.
        const uint8_t* mediaKey;    // 2-byte media report, e.g. KEY_MEDIA_PLAY, KEY_MEDIA_PAUSE, etc.
    } keyCode;

    // Constructors
    DuckyKey(uint8_t key, bool isMod = false)
        : type(isMod ? KeyType::Modifier : KeyType::Standard) {
        if (isMod)
            keyCode.modifierKey = key;
        else
            keyCode.standardKey = key;
    }

    DuckyKey(const uint8_t* media)
        : type(KeyType::Media) {
        keyCode.mediaKey = media;
    }
};

class DuckyEngine {
    public:
        DuckyEngine(BleKeyboard &bleKeyboard);
        void executeScript(const std::string &script);

    private:
        int intervalDelay;              // Delay interval between key presses.
        BleKeyboard &bleKeyboard;       // Reference to the BLE keyboard instance.
        std::vector<uint8_t> activeKeys;       // List of currently pressed keys.
        std::vector<uint8_t> activeModifiers;  // List of currently active modifier keys.
        std::vector<const uint8_t*> activeMediaKeys;  // List of currently active media keys.
        std::map<std::string, uint8_t> keyMap; // Mapping of key names to HID key codes.
        std::map<std::string, const uint8_t*> mediaKeyMap; // Mapping of media key names to HID key codes.

        void processCommand(const std::string &line);
        std::string extractString(std::istringstream &ss);
        void handleModifierCombo(std::string key, std::istringstream &ss);
        void pressKey(const std::string &key, bool allowLog);
        void releaseKey(const std::string &key, bool allowLog);
        DuckyKey getKey(const std::string& key);
        void sendKey(const std::string& key);
        void typeString(const std::string &text);
        void typePrint(const std::string &text);
        void delayExecution(int milliseconds);
        bool isModifierKey(uint8_t keyCode);
        void removeModifier(uint8_t keyCode);
        void removeMediaKey(const uint8_t* keyCode);
        void removeKey(uint8_t keyCode);
        void releaseAllKeys();
};

#endif 