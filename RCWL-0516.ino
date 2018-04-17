/*
 * ESP8266_RCWL-0516
 * 
 * Note: The ESP8266 is not 5V tolerant but the RCL-0516 requires 5V!
 * Logic level conversioin (e.g. voltage divider) must be used on
 * the output pin of the RCWL-0516 sensor.
 * 
 * Pin and wiring connections:
 * RCWL-0516       Wemos D1 Mini
 * ---------       -------------
 * Vcc             5V         
 * OUT             D5   !!Voltage Divider Required!!
 * GND             GND

 * Voltage Divider
 * RCWL-0516         Wemos D1 Mini
 *  OUT -- 10k Ohm --+--  D5     
 *                   |
 *                15k Ohm
 *                   |
 *                  GND
 *                  
 * Frank Milburn  May 2018
 */

const int radarPin = D5;
boolean lastRadarState = 0;
unsigned long radarCount = 0;
 
void setup() {
  pinMode(radarPin, INPUT);
  digitalRead(radarPin);
  delay(5000);                  // let things settle

  Serial.begin(115200);
  Serial.println("Starting RCWL-0516 Test...");
}

void loop() {
  boolean radarTrap = false;
  radarTrap = digitalRead(radarPin);
  if (radarTrap == true){
    if (lastRadarState == false){
      radarCount++;
      lastRadarState = true;
      Serial.print("Motion detected: ");
      Serial.println(radarCount);
    }
  }
  else {
    lastRadarState = false;
  }
}
