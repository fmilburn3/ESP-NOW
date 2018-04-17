/*
 * Sends a C struct from ESP8266 to ESP8266 using ESP-NOW
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

// Insert the MAC Address of the receiver ESP(s) that will receive the messages
uint8_t recMAC[] = {0x5E, 0xCF, 0x7F, 0xF8, 0x4D, 0x62};

#define WIFI_CHANNEL 1                                                         // Set channel from 1 to 14                                                        

// Set up a struct to store the text and numbers that will be sent - use packed for the smallest possible alignment for bytes
// All receiving ESP(s) must also have this structure.

#define MSG_LEN 15
struct __attribute__((packed)) MSG {                                          
  char text[MSG_LEN];
  unsigned long number;
} msg;
int msgSize = sizeof(msg);                                                      // NOTE:  must be less than 250 bytes!

void setup() {
  Serial.begin(115200);
  Serial.println("\nStarting ESP-NOW Sender");
  
  WiFi.mode(WIFI_STA);                                                          // Place in station mode for ESP-NOW sensor node
  WiFi.disconnect();                                                            // Make sure disconnected from WiFi

  Serial.print("Sender MAC:   ");                                               // Serial print sender MAC
  Serial.println(WiFi.macAddress().c_str());
  Serial.print("Receiver MAC: ");                                               // Serial print receiver MAC
  for (int i = 0; i < 6; i++){
    Serial.print(recMAC[i], HEX);
    if (i < 5){
      Serial.print(":");
    }
  }
  Serial.print(", channel: ");                                                  // Serial print the channel
  Serial.println(WIFI_CHANNEL);
  Serial.println("");

  if (esp_now_init() != 0) {                                                    // Halt execution if init failed
    Serial.println("ESP-NOW init failed");
    while(1);
  }
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);                               // Sender is in controller role
  esp_now_add_peer(recMAC, ESP_NOW_ROLE_SLAVE, WIFI_CHANNEL, NULL, 0);          // Sends to MAC of receiver in slave role
                                                                                // Note that multiple peers can be added
  strcpy(msg.text, "Hello World: ");                                            // Initialize the text message                                 
  msg.number = 0;                                                               // and number to be sent
}

void loop() {
  Serial.print(msg.text);
  Serial.println(msg.number);                                                                                                
  unsigned char store[msgSize];                                                 // messages are stored as unsigned char
  memcpy(&store, &msg, msgSize);                                                // for transmission (req'd by ESP-NOW)
  esp_now_send(NULL, store, msgSize);                                           // NULL sends to all peers

  delay(5000);
  msg.number++;                                                                 // msg number is just a loop counter
}
