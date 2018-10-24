/*
 * Receives a C struct from another ESP8266 using ESP-NOW
 * NOTE: Security is not set in this example
 * 
 * Frank Milburn, April 2018
 * Tested with Arduino 1.8.4 and Wemos D1 mini
 *
 * Based on code from the following:
 * https://github.com/SensorsIot/ESP-Now-Tests by Andreas Spiess
 * https://github.com/HarringayMakerSpace/ESP-Now by Anthony Elder
 */
#include <ESP8266WiFi.h>
extern "C" {
#include <espnow.h>
}

String senderMAC;                                                              // sender's MAC
#define WIFI_CHANNEL 1                                                         // Set channel from 1 to 14 (same as sender)                                                       

// Set up a struct to store the text and numbers that will be sent - use packed for the smallest possible alignment for bytes
// The sending ESP(s) must also have this structure.

#define MSG_LEN 40
struct __attribute__((packed)) MSG {                                          
  char text[MSG_LEN];
  unsigned long number;
} msg;
int msgSize = sizeof(msg);                                                      // NOTE:  must be less than 250 bytes!

volatile boolean haveMsg = false;                                               // Volatile flag when new msg is received

void setup() {
  Serial.begin(115200); 
  Serial.println("\nStarting ESP_Now Receiver");

  WiFi.mode(WIFI_AP);                                                           // AP - cannot put radio to sleep
  Serial.print("Receiver MAC: ");
  Serial.println(WiFi.macAddress());
  Serial.println("");

  if (esp_now_init() != 0) {                                                    // Halt execution if init failed
    Serial.println("ESP-NOW init failed");
    while(1);
  }
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);                                    // slave is receiver only
  esp_now_register_recv_cb([](uint8_t *mac, uint8_t *data, uint8_t len) {       // call back (interrupt) when
    senderMAC = "";                                                             // message is received stores
    for (int i = 0; i < 6; i++){                                                // the sender's MAC
      senderMAC += String(mac[i], HEX);
      if (i < 5){
        senderMAC += ":";
      }
    }
    memcpy(&msg, data, sizeof(msg));                                            // call back also stores the msg
    haveMsg = true;                                                             // flag that msg was received
  });
}

void loop() {
  if (haveMsg == true){                                                         // If a msg has been received
    Serial.print("Device ");                                                    // print the MAC and the msg
    Serial.print(senderMAC);
    Serial.print(" reports \"");
    Serial.print(msg.text);
    Serial.print(msg.number);
    Serial.println("\"");
    
    haveMsg = false;                                                            // and set the msg flag to false
  }
}

