#include <BleKeyboard.h>
#include <DuckyEngine.h>

BleKeyboard bleKeyboard("Ducky Board", "Trident Apollo", 100);
DuckyEngine macro(bleKeyboard);

const char* defaultDuckyScript = R"rawliteral(
GUI r
DELAY 200
DELAY INTERVAL 15
PRINT "cmd"
DELAY 200
ENTER
DELAY 2000
PRINT "echo 'Opening Task Manager'"
DELAY 200
ENTER
DELAY 500
CTRL ALT DELETE
DELAY 1000
DOWN
DELAY 200
DOWN
DELAY 200
DOWN
DELAY 400
ENTER
)rawliteral";

void connect();

void setup() {
    Serial.begin(115200);
    connect();
    
    Serial.println("Type 'go' to execute the default Ducky Script.");
    Serial.println("Send a custom Ducky Script via Serial input to execute it.");
}

void loop() {
    if (Serial.available()) {
        String input = Serial.readStringUntil('\n');
        input.trim();
        
        if (input.equalsIgnoreCase("go")) {
            Serial.println("Executing Default Ducky Script...");
            macro.executeScript(defaultDuckyScript);
            Serial.println("Execution Finished!");
        } else {
            Serial.println("Executing Custom Ducky Script...");
            macro.executeScript(input.c_str());
            Serial.println("Execution Finished!");
        }
    }
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
