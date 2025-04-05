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
1. Download the latest [release](https://github.com/TriDEntApollO/EPS32-DuckyEngine/releases) or download the [code](https://github.com/TriDEntApollO/ESP32-BLE-Keyboard-V2/archive/refs/heads/main.zip) as a ZIP file for latest commits.
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

#### Basic Keys
| Basic Keys        | Description                       |
|-------------|-----------------------------------|
| `ENTER`     | Presses the Enter/Return key      |
| `ESC`       | Presses the Escape key            |
| `TAB`       | Presses the Tab key               |
| `SPACE`     | Presses the Space key             |
| `BACKSPACE` | Presses the Backspace key         |
| `DELETE`    | Presses the Delete key            |

#### Modifier Keys
| Key           | Description                         |
|---------------|-------------------------------------|
| `CTRL`        | Holds down the Control key          |
| `SHIFT`       | Holds down the Shift key            |
| `ALT`         | Holds down the Alt key              |
| `GUI`         | Holds down the Windows/Command key  |
| `LEFT_CTRL`   | Holds down the Left Control key     |
| `LEFT_SHIFT`  | Holds down the Left Shift key       |
| `LEFT_ALT`    | Holds down the Left Alt key         |
| `LEFT_GUI`    | Holds down the Left GUI key         |
| `RIGHT_CTRL`  | Holds down the Right Control key    |
| `RIGHT_SHIFT` | Holds down the Right Shift key      |
| `RIGHT_ALT`   | Holds down the Right Alt key        |
| `RIGHT_GUI`   | Holds down the Right GUI key        |

#### Navigation Keys
| Key         | Description                      |
|-------------|----------------------------------|
| `UP`        | Presses the Up Arrow key         |
| `DOWN`      | Presses the Down Arrow key       |
| `LEFT`      | Presses the Left Arrow key       |
| `RIGHT`     | Presses the Right Arrow key      |
| `INSERT`    | Presses the Insert key           |
| `HOME`      | Presses the Home key             |
| `END`       | Presses the End key              |
| `PAGE_UP`   | Presses the Page Up key          |
| `PAGE_DOWN` | Presses the Page Down key        |

#### Function Keys
| Key          | Description                           |
|--------------|---------------------------------------|
| `F1 to F24`  | Presses Function keys F1 through F24  |


#### Alphanumeric Cluster
| Key         | Description                      |
|-------------|----------------------------------|
| `CAPS_LOCK` | Presses the Caps Lock key        |
| `MENU`      | Presses the Menu key             |

#### Function Control Cluster
| Key            | Description                    |
|----------------|--------------------------------|
| `PRINT_SCREEN` | Presses the Print Screen key   |
| `SCROLL_LOCK`  | Presses the Scroll Lock key    |
| `PAUSE`        | Presses the Pause key          |

#### Keypad Keys
| Key           | Description                         |
|---------------|-------------------------------------|
| `NUM_LOCK`    | Presses the Num Lock key            |
| `KP_SLASH`    | Presses the Numpad Slash key        |
| `KP_ASTERISK` | Presses the Numpad Asterisk key     |
| `KP_MINUS`    | Presses the Numpad Minus key        |
| `KP_PLUS`     | Presses the Numpad Plus key         |
| `KP_ENTER`    | Presses the Numpad Enter key        |
| `KP_1`        | Presses the Numpad 1 key            |
| `KP_2`        | Presses the Numpad 2 key            |
| `KP_3`        | Presses the Numpad 3 key            |
| `KP_4`        | Presses the Numpad 4 key            |
| `KP_5`        | Presses the Numpad 5 key            |
| `KP_6`        | Presses the Numpad 6 key            |
| `KP_7`        | Presses the Numpad 7 key            |
| `KP_8`        | Presses the Numpad 8 key            |
| `KP_9`        | Presses the Numpad 9 key            |
| `KP_0`        | Presses the Numpad 0 key            |
| `KP_DOT`      | Presses the Numpad Decimal key      |

#### Media Keys
| Key                                    | Description                          |
|----------------------------------------|--------------------------------------|
| `MEDIA_PLAY`                           | Toggles play/pause                   |
| `MEDIA_PAUSE`                          | Toggles play/pause                   |
| `MEDIA_STOP`                           | Stops media playback                 |
| `MEDIA_PLAY_PAUSE`                     | Toggles play/pause                   |
| `MEDIA_NEXT_TRACK`                     | Skips to next track                  |
| `MEDIA_PREVIOUS_TRACK`                 | Returns to previous track            |
| `MEDIA_MUTE`                           | Toggles mute                         |
| `MEDIA_VOLUME_UP`                      | Increases volume                     |
| `MEDIA_VOLUME_DOWN`                    | Decreases volume                     |
| `MEDIA_WWW_HOME`                       | Opens web browser home               |
| `MEDIA_LOCAL_MACHINE_BROWSER`          | Opens local browser                  |
| `MEDIA_CALCULATOR`                     | Opens calculator                     |
| `MEDIA_WWW_BOOKMARKS`                  | Opens bookmarks                      |
| `MEDIA_WWW_SEARCH`                     | Opens web search                     |
| `MEDIA_WWW_STOP`                       | Stops web page load                  |
| `MEDIA_WWW_BACK`                       | Navigates back in browser            |
| `MEDIA_CONSUMER_CONTROL_CONFIGURATION` | Opens consumer control settings      |
| `MEDIA_EMAIL_READER`                   | Opens email client                   |



## Credits
This project is built upon **T-vK's [ESP32-BLE-Keyboard](https://github.com/T-vK/ESP32-BLE-Keyboard)** library, which handles the BLE keyboard functionality.
