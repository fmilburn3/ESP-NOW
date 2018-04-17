/*
 * ESP8266_PIR
 * 
 * Note: The ESP8266 is not 5V tolerant but the PIR requires 5V!
 * Logic level conversioin (e.g. voltage divider) must be used on
 * the output pin of the PIR sensor.
 * 
 * Pin and wiring connections:
 * PIR       Wemos D1 Mini
 * -------   -------------
 * Vcc       5V         
 * OUT       D1   !!Voltage Divider Required!!
 * GND       GND

 * Voltage Divider
 *  PIR              Wemos D1 Mini
 *  OUT -- 10k Ohm --+--  D1     
 *                   |
 *                15k Ohm
 *                   |
 *                  GND
 *                  
 * Frank Milburn   May 2018
 */

const int pirPin = D1;
boolean lastPirState = 0;
unsigned long pirCount = 0;
 
void setup() {
  pinMode(pirPin, INPUT);
  digitalRead(pirPin);
  delay(10);                  // let things settle

  Serial.begin(115200);
  Serial.println("Starting HC-SR04 Test...");
}

void loop() {
  boolean pirTrip;
  pirTrip = digitalRead(pirPin);
  if (pirTrip == true){
    if (lastPirState == false){
      pirCount++;
      lastPirState = true;
      Serial.print("Motion detected: ");
      Serial.println(pirCount);
    }
  }
  else {
    lastPirState = false;
  }
}
