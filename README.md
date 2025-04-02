# ESP32-DuckyEngine 🦆

ESP32-DuckyEngine is a lightweight **Ducky Script interpreter** for the ESP32 with **BLE keyboard** support. It enables the ESP32 to emulate a Bluetooth keyboard and execute scripted keystrokes, making it perfect for automation, macros, and penetration testing.

## Features
- ✅ **Ducky Script Support** – Execute scripts with standard Ducky commands
- ✅ **Bluetooth HID Keyboard** – Emulates a BLE keyboard for seamless input
- ✅ **Custom Macro Engine** – Send keystrokes, type text, and trigger shortcuts
- ✅ **Configurable Delays & Intervals** – Fine-tune script execution speed


## Getting Started

### 1️⃣ Prerequisites
- **ESP32 Board**
- **Arduino IDE** or **PlatformIO**
- **BleKeyboard Library** - [use this](https://github.com/TriDEntApollO/ESP32-BLE-Keyboard-V2) for updated fixes as the original is not being maintianed anymore
- **Make sure you can use the ESP32 with the Arduino IDE. Instructions can be found [here](https://docs.espressif.com/projects/arduino-esp32/en/latest/).**

### 2️⃣ Installation
1. Download the latest [release](https://github.com/TriDEntApollO/ESP32-BLE-Keyboard-V2/releases) or download the [code](https://github.com/TriDEntApollO/ESP32-BLE-Keyboard-V2/archive/refs/heads/main.zip) as a ZIP file for latest commits.
2. In the Arduino IDE go to `"Sketch" -> "Include Library" -> "Add .ZIP Library..."` and select the `ZIP` file you just downloaded.
3. Restart Arduino IDE to recognize the new library.
4. You can now go to `"File" -> "Examples" -> "ESP32 DuckyEngine"` and select any of the examples to get started.


### 3️⃣ Example Usage
Here's a simple example that executes a Ducky Script on connection:

```cpp
#include <BleKeyboard.h>
#include "DuckyEngine.h"

BleKeyboard bleKeyboard("ESP32 Ducky", "TridentApollo", 100);
DuckyEngine macro(bleKeyboard);

const char* duckyScript = R"rawliteral(
GUI r
DELAY 200
STRING notepad
ENTER
DELAY 500
STRING Hello, this is an automated script!
ENTER
)rawliteral";

void setup() {
    Serial.begin(115200);
    bleKeyboard.begin();
    while (!bleKeyboard.isConnected()) {
        delay(500);
    }
    Serial.println("Executing Script...");
    macro.executeScript(duckyScript);
    Serial.println("Execution Finished!");
}

void loop() {
    // No loop function needed
}
```

### 4️⃣ Available Ducky Commands
| Command          | Description                                                    |
|------------------|----------------------------------------------------------------|
| `PRESS`          | Presses a specified key without releasing it                   |
| `RELEASE`        | Releases a specified key or all keys                           |
| `RELEASE ALL`    | Releases all currently held keys                               |
| `STRING`         | Types a string instantly                                       |
| `PRINT`          | Types a string with a per-character delay                      |
| `ENTER`          | Presses the Enter key                                          |
| `DELAY`          | Waits for a specified time (in milliseconds)                   |
| `DELAY INTERVAL` | Sets delay between each cahrater for `PRINT` (in milliseconds) |

### 5️⃣ Available Keys
| Key        | Description                                     |
|------------|-------------------------------------------------|
| `CTRL`     | Holds down the Control key                      |
| `ALT`      | Holds down the Alt key                          |
| `SHIFT`    | Holds down the Shift key                        |
| `GUI`      | Holds down the Windows/Command key              |
| `TAB`      | Presses the Tab key                             |
| `ESC`      | Presses the Escape key                          |
| `SPACE`    | Presses the Space key                           |
| `BACKSPACE`| Presses the Backspace key                       |
| `DELETE`   | Presses the Delete key                          |
| `UP`       | Presses the Up Arrow key                        |
| `DOWN`     | Presses the Down Arrow key                      |
| `LEFT`     | Presses the Left Arrow key                      |
| `RIGHT`    | Presses the Right Arrow key                     |

## Credits
This project is built upon **T-vK's [ESP32-BLE-Keyboard](https://github.com/T-vK/ESP32-BLE-Keyboard)** library, which handles the BLE keyboard functionality.
