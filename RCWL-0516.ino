/*
 * ESP8266_RCWL-0516
 *  
 * Pin and wiring connections:
 * RCWL-0516       Wemos D1 Mini
 * ---------       -------------
 * Vcc             5V         
 * OUT             D5 
 * GND             GND
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
