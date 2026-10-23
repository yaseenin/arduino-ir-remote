#include <Arduino.h>

// Define IR LED Pin (Pin 3 supports hardware PWM timer on Arduino Uno)
#define IR_SEND_PIN 3 
#include <IRremote.h>

// --- LG TV CONFIG (NEC Protocol) ---
// Standard LG TV address is typically 0x04
#define LG_TV_ADDRESS   0x04
#define LG_CMD_POWER    0x08
#define LG_CMD_VOL_UP   0x02

// --- VOLTAS AC CONFIG (Raw Timing Array) ---
// Raw timing array representing a Voltas AC command (e.g., Power On / 24°C Cool)
// Note: If your specific Voltas model differs, record your remote using an IR receiver 
// and replace this sample array with your captured timing values.
uint16_t voltasAcCommand[] = {
    8950, 4400, 600, 1650, 600, 550, 600, 550, 600, 1650, 
    600, 550, 600, 550, 600, 550, 600, 1650, 600, 1650, 
    600, 550, 600, 1650, 600, 550, 600, 550, 600, 550, 
    600, 1650, 600, 550, 600, 1650, 600, 1650, 600, 1650, 
    600, 550, 600, 550, 600, 550, 600, 550, 600, 550, 
    600, 550, 600, 550, 600, 1650, 600, 550, 600, 550, 
    600, 1650, 600, 1650, 600, 1650, 600, 1650, 600
};

void setup() {
    Serial.begin(115200);
    while (!Serial); // Wait for Serial Monitor to initialize
    
    // Initialize IR sender on Pin 3
    IrSender.begin(IR_SEND_PIN);
    
    Serial.println(F("=========================================="));
    Serial.println(F("   Universal IR Remote Controller Active  "));
    Serial.println(F("=========================================="));
    Serial.println(F("Type a command in the Serial Monitor:"));
    Serial.println(F("  [1] -> Send LG TV Power Toggle"));
    Serial.println(F("  [2] -> Send LG TV Volume Up"));
    Serial.println(F("  [3] -> Send Voltas AC Command (Raw)"));
    Serial.println(F("------------------------------------------"));
}

void loop() {
    if (Serial.available() > 0) {
        char input = Serial.read();
        
        switch (input) {
            case '1':
                Serial.println(F("-> Sending LG TV Power command..."));
                // sendNEC(address, command, numberOfRepeats)
                IrSender.sendNEC(LG_TV_ADDRESS, LG_CMD_POWER, 0);
                break;
                
            case '2':
                Serial.println(F("-> Sending LG TV Volume Up command..."));
                IrSender.sendNEC(LG_TV_ADDRESS, LG_CMD_VOL_UP, 0);
                break;
                
            case '3':
                Serial.println(F("-> Sending Voltas AC command..."));
                // sendRaw(array, arrayLength, frequencyInkHz)
                IrSender.sendRaw(voltasAcCommand, sizeof(voltasAcCommand) / sizeof(voltasAcCommand[0]), 38);
                break;
                
            default:
                if (input != '\n' && input != '\r') {
                    Serial.println(F("Invalid selection. Press '1', '2', or '3'."));
                }
                break;
        }
        
        delay(300); // Prevent accidental button spamming
    }
}