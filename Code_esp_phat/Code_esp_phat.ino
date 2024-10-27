#include <esp_now.h>
#include <WiFi.h>
 // MAC Address of responder - edit as required
uint8_t broadcastAddress[] = {0x24, 0x62, 0xAB, 0xE6, 0xA7, 0x3C};
const int button1Pin = 32; // GPIO của BTN1
const int button2Pin = 33; // GPIO của BTN2
// Define a data structure
typedef struct struct_message {
    bool btn1State;
    bool btn2State;
} struct_message;
 
// Create a structured object
struct_message myData;
 
// Peer info
esp_now_peer_info_t peerInfo;
 
// Callback function called when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void IRAM_ATTR handleButton1Interrupt() {
    myData.btn1State = !myData.btn1State;
    esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
}

void IRAM_ATTR handleButton2Interrupt() {
    myData.btn2State = !myData.btn2State;
    esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
}

 
void setup() {
  
  // Set up Serial Monitor
  Serial.begin(115200);
 
  // Set ESP32 as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
 
  // Initilize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
 
  // Register the send callback
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

  pinMode(button1Pin, INPUT_PULLUP);
  pinMode(button2Pin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(button1Pin), handleButton1Interrupt, FALLING);
  attachInterrupt(digitalPinToInterrupt(button2Pin), handleButton2Interrupt, FALLING);
}
 
void loop() {  

}






// #include <esp_now.h>
// #include <WiFi.h>
// #include <EEPROM.h>

// #define EEPROM_SIZE sizeof(struct_message)

// // MAC Address of responder - edit as required
// uint8_t broadcastAddress[] = {0x24, 0x62, 0xAB, 0xE6, 0xA7, 0x3C};
// const int button1Pin = 32; // GPIO for BTN1
// const int button2Pin = 33; // GPIO for BTN2

// // Define a data structure
// typedef struct struct_message {
//     bool btn1State;
//     bool btn2State;
// } struct_message;

// // Create a structured object
// struct_message myData;

// // Peer info
// esp_now_peer_info_t peerInfo;

// // Callback function called when data is sent
// void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
//     Serial.print("\r\nLast Packet Send Status:\t");
//     Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
// }

// volatile bool dataChanged = false; // Cờ để đánh dấu thay đổi dữ liệu

// void IRAM_ATTR handleButton1Interrupt() {
//     myData.btn1State = !myData.btn1State;
//     dataChanged = true; // Đánh dấu rằng dữ liệu đã thay đổi
// }

// void IRAM_ATTR handleButton2Interrupt() {
//     myData.btn2State = !myData.btn2State;
//     dataChanged = true; // Đánh dấu rằng dữ liệu đã thay đổi
// }

// void saveDataToEEPROM() {
//     EEPROM.put(0, myData);
//     EEPROM.commit(); // Ghi dữ liệu vào EEPROM
// }

// void loadDataFromEEPROM() {
//     EEPROM.get(0, myData);
// }

// void setup() {
//     Serial.begin(115200);
//     WiFi.mode(WIFI_STA);
//     EEPROM.begin(EEPROM_SIZE);
//     loadDataFromEEPROM(); // Load previous state from EEPROM

//     if (esp_now_init() != ESP_OK) {
//         Serial.println("Error initializing ESP-NOW");
//         return;
//     }

//     esp_now_register_send_cb(OnDataSent);
//     memcpy(peerInfo.peer_addr, broadcastAddress, 6);
//     peerInfo.channel = 0;
//     peerInfo.encrypt = false;

//     if (esp_now_add_peer(&peerInfo) != ESP_OK) {
//         Serial.println("Failed to add peer");
//         return;
//     }

//     pinMode(button1Pin, INPUT_PULLUP);
//     pinMode(button2Pin, INPUT_PULLUP);
//     attachInterrupt(digitalPinToInterrupt(button1Pin), handleButton1Interrupt, FALLING);
//     attachInterrupt(digitalPinToInterrupt(button2Pin), handleButton2Interrupt, FALLING);
// }

// void loop() {
//     if (dataChanged) {
//         dataChanged = false; // Reset cờ
//         saveDataToEEPROM(); // Lưu trạng thái vào EEPROM
//         esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData)); // Gửi dữ liệu
//         delay(10); // Thêm một độ trễ ngắn
//     }
// }