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
#include <sstream>
#include <BleKeyboard.h>

class DuckyEngine {
public:
    DuckyEngine(BleKeyboard &bleKeyboard);
    void executeScript(const std::string &script);

private:
    int intervalDelay;              // Delay interval between key presses.
    BleKeyboard &bleKeyboard;       // Reference to the BLE keyboard instance.
    std::vector<uint8_t> activeKeys;       // List of currently pressed keys.
    std::vector<uint8_t> activeModifiers;  // List of currently active modifier keys.
    std::map<std::string, uint8_t> keyMap; // Mapping of key names to HID key codes.

    void processCommand(const std::string &line);
    std::string extractString(std::istringstream &ss);
    void pressKey(const std::string &key, bool allowModifiers);
    void releaseKey(const std::string &key, bool allowModifiers);
    void typeString(const std::string &text);
    void typePrint(const std::string &text);
    void delayExecution(int milliseconds);
    bool isModifierKey(uint8_t keyCode);
    void removeModifier(uint8_t keyCode);
    void removeKey(uint8_t keyCode);
    void releaseAllKeys();
};

#endif 