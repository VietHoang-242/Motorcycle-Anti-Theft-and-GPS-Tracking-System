#include <SIM_GPS.h>
#include <esp_now.h>
#include <WiFi.h>

const int led_red = 33;    
const int led_green = 32; 
const int key = 27;
const int buzzer = 5;

#define MCU_SIM_TX_PIN 17
#define MCU_SIM_RX_PIN 16
#define MCU_SIM_EN_PIN 15
#define PHONE_NUMBER "0528169490" 
SIM_GPS simgps(Serial2, MCU_SIM_TX_PIN, MCU_SIM_RX_PIN, MCU_SIM_EN_PIN);

typedef struct struct_message {
    bool btn1State;
    bool btn2State;
} struct_message;
struct_message myData;

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
    memcpy(&myData, incomingData, sizeof(myData));
    digitalWrite(key, myData.btn1State);    
    digitalWrite(buzzer, myData.btn2State);    
}

void setup() {
    simgps.begin();

    pinMode(led_red, OUTPUT);
    pinMode(led_green, OUTPUT);
    pinMode(key, OUTPUT);
    pinMode(buzzer, OUTPUT);

    WiFi.mode(WIFI_STA);
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }
    esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
    // simgps.sendSMS(PHONE_NUMBER,"test");
    simgps.getGPSData();   
    
}