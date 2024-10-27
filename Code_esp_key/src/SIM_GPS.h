#ifndef SIM_GPS_H
#define SIM_GPS_H

#include <HardwareSerial.h>

class SIM_GPS {
public:
    SIM_GPS(HardwareSerial &serial, int txPin, int rxPin, int enPin);
    void begin();
    void getGPSData();
    void sendSMS(const String &phoneNumber, const String &message);
    void loop();

private:
    void simAtWait();
    bool simAtCmd(const String &cmd);
    bool simAtSend(char c);
    
    HardwareSerial &simSerial;
    int MCU_SIM_EN_PIN;
    int MCU_SIM_TX_PIN;
    int MCU_SIM_RX_PIN;
};

SIM_GPS::SIM_GPS(HardwareSerial &serial, int txPin, int rxPin, int enPin)
    : simSerial(serial), MCU_SIM_EN_PIN(enPin), MCU_SIM_TX_PIN(txPin), MCU_SIM_RX_PIN(rxPin) {
    pinMode(MCU_SIM_EN_PIN, OUTPUT); 
    digitalWrite(MCU_SIM_EN_PIN, LOW);
}

void SIM_GPS::begin() {
    delay(20);
    
    Serial.begin(115200);
    Serial.println("\n\nSystem started!!!!");
    
    delay(8000);
    simSerial.begin(115200, SERIAL_8N1, MCU_SIM_RX_PIN, MCU_SIM_TX_PIN);

    simAtCmd("AT");
    simAtCmd("ATI");
    simAtCmd("AT+CPIN?");
    simAtCmd("AT+CSQ");
    simAtCmd("AT+CIMI");
    simAtCmd("AT+QGPS=1");
    delay(5000);
    getGPSData();
}

void SIM_GPS::getGPSData() {
    simAtWait();
    // simAtCmd("AT+QGPS=1"); 
    // delay(10000);
    simAtCmd("AT+QGPS?");
    simAtCmd("AT+QGPSLOC=0");
}

void SIM_GPS::sendSMS(const String &phoneNumber, const String &message) {
    simAtCmd("AT+CMGF=1");
    String temp = "AT+CMGS=\"" + phoneNumber + "\"";
    simAtCmd(temp);
    simAtCmd(message);
    simAtSend(0x1A);
}

void SIM_GPS::simAtWait() {
    delay(100);
    while (simSerial.available()) {
        Serial.write(simSerial.read());
    }
}

bool SIM_GPS::simAtCmd(const String &cmd) {
    Serial.print("Sending command: ");
    Serial.println(cmd);
    simSerial.println(cmd);
    delay(500);
    simAtWait();
    return true;
}

bool SIM_GPS::simAtSend(char c) {
    simSerial.write(c);
    return true;
}

// void SIM_GPS::loop() {
//     if (Serial.available()) {
//         char c = Serial.read();
//         simSerial.write(c);
//     }
//     simAtWait();
//     simAtCmd("AT+QGPS?");
//     simAtCmd("AT+QGPSLOC=0");
// }
#endif 

