#include <BleKeyboard.h>
#include <DuckyEngine.h>


BleKeyboard bleKeyboard("Ducky Board", "Trident Apollo", 100);
DuckyEngine macro(bleKeyboard);

const char* duckyScript = R"rawliteral(
GUI r
DELAY 200
DELAY INTERVAL 15
PRINT "cmd"
DELAY 200
ENTER
DELAY 2000
PRINT "echo 'Hello, World!'"
DELAY 200
ENTER
)rawliteral";

void connect();

void setup() {
    Serial.begin(115200);

    connect();
    Serial.println("Executing Ducky Script...");
    macro.executeScript(duckyScript);
    Serial.println("Executiion Finished!");
}

void loop() {

}

void connect() {
    Serial.println("Starting BLE work!");
    bleKeyboard.begin();
    
    for (int i = 0; !bleKeyboard.isConnected(); i++) {
        Serial.print("Connecting");  
        for (int j = 0; j < (i % 4); j++) {
            Serial.print(".");
        }
        delay(500);
    }
    Serial.println("Connected Successfully");
}