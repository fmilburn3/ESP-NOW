/*
 * Reads Radar, PIR, and distance sensors and then sends C struct messages from ESP8266 to ESP8266 using ESP-NOW
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
uint8_t recMAC[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
#define WIFI_CHANNEL 1                                                         // Set channel from 1 to 14                                                        

// Set up a struct to store the text and numbers that will be sent - use packed for the smallest possible alignment for bytes
// All receiving ESP(s) must also have this structure.
#define MSG_LEN 40
struct __attribute__((packed)) MSG {                                          
  char text[MSG_LEN];
  unsigned long value;
  unsigned int led;
};
MSG radarMsg;
MSG pirMsg;
MSG sonicMsg;
int msgSize = sizeof(radarMsg);                                                  // NOTE:  must be less than 250 bytes!

// Radar
const unsigned int radarPin = D5;
boolean            lastRadarState = 0;
unsigned long      radarCount = 0;

// PIR
const unsigned int pirPin = D1;
boolean            lastPirState = 0;
unsigned long      pirCount = 0;

// Ultrasonic
const unsigned int trigPin = D3;
const unsigned int echoPin = D2;
unsigned long      lastEchoDistance = 0;

void setup() {
  // Serial
  Serial.begin(115200);
  Serial.println("\n\nStarting ESP-NOW Sender");
  Serial.print  ("Message size (bytes) is: ");
  Serial.println(msgSize);
  Serial.println("");

  // ESP-NOW
  WiFi.mode(WIFI_STA);                                                          // Place in station mode for ESP-NOW sender node
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
  Serial.print("\nChannel: ");                                                  // Serial print the channel
  Serial.println(WIFI_CHANNEL);
  Serial.println("");

  if (esp_now_init() != 0) {                                                    // Halt execution if init failed
    Serial.println("ESP-NOW init failed");
    while(1);
  }
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);                               // Sender is in controller role
  esp_now_add_peer(recMAC, ESP_NOW_ROLE_SLAVE, WIFI_CHANNEL, NULL, 0);          // Sends to MAC of receiver in slave role
                                                                                // Note that multiple peers can be added
  
  // Radar
  pinMode(radarPin, INPUT);
  digitalRead(radarPin);
  strcpy(radarMsg.text, "Radar Detection: ");                                   // Initialize the text messages                                
  radarMsg.value = 0;                                                           // and values to be sent
  radarMsg.led = 0;
  Serial.println("Radar set up");

  // PIR
  pinMode(pirPin, INPUT);
  digitalRead(pirPin);
  strcpy(pirMsg.text, "PIR Activity: ");
  pirMsg.value = 0;
  pirMsg.led = 1;
  Serial.println("PIR set up");

  // Ultrasonic Distance
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);  
  strcpy(sonicMsg.text, "Ultrasonic Distance (cm): ");
  sonicMsg.value = 0;
  sonicMsg.led = 2;
  Serial.println("Ultrasonic Sensor set up");

  delay(5000);                  // let things settle
  Serial.println("");
}

void loop() {

  unsigned char store[msgSize];                                                     // ESP-NOW buffer stored as unsigned char
  
  // Radar
  boolean radarTrap = false;
  radarTrap = digitalRead(radarPin);
  if (radarTrap == true){
    if (lastRadarState == false){
      radarCount++;
      lastRadarState = true;
      Serial.print(radarMsg.text);
      Serial.println(radarMsg.value);
      memcpy(&store, &radarMsg, msgSize);                                           // for transmission (req'd by ESP-NOW)
      esp_now_send(NULL, store, msgSize);                                           // NULL sends to all peers
      radarMsg.value++;                                                             // msg value is just a counter
    }
  }
  else {
    lastRadarState = false;
  }

  // PIR
  boolean pirTrip;
  pirTrip = digitalRead(pirPin);
  if (pirTrip == true){
    if (lastPirState == false){
      pirCount++;
      lastPirState = true;
      Serial.print(pirMsg.text);
      Serial.println(pirMsg.value);
      memcpy(&store, &pirMsg, msgSize);                                             // for transmission (req'd by ESP-NOW)
      esp_now_send(NULL, store, msgSize);                                           // NULL sends to all peers
      pirMsg.value++;                                                               // msg value is just a counter
    }
  }
  else {
    lastPirState = false;
  }
  
  // Ultrasonic
  if ((lastRadarState == true) || (lastPirState == true)){                          // check for change in distance
    unsigned long pulseLength;
    digitalWrite(trigPin, LOW);                                                     // send low to get a clean pulse
    delayMicroseconds(5);                                                           // let it settle
    digitalWrite(trigPin, HIGH);                                                    // send high to trigger device
    delayMicroseconds(10);                                                          // let it settle
    pulseLength = pulseIn(echoPin, HIGH);                                           // measure pulse coming back
    sonicMsg.value = pulseLength / 58;                                              // calculate distance (cm)
    if ((sonicMsg.value - lastEchoDistance) > 10){                                  // check for change in distance
      if ((sonicMsg.value > 0) && (sonicMsg.value < 500)){                          // check for reasonable values
        lastEchoDistance = sonicMsg.value;
        Serial.print(sonicMsg.text);
        Serial.println(sonicMsg.value);
        memcpy(&store, &sonicMsg, msgSize);                                         // for transmission (req'd by ESP-NOW)
        esp_now_send(NULL, store, msgSize);                                         // NULL sends to all peers
      }
    }
  }
  delay(100);
}
