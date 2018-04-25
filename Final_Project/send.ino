/*
 * ESP8266_RCWL-0516
 * Adds ability to read Pin 12 on the RCWL-9196 IC which is located on the RCWL-0516 sensor module
 * Displays Analog on the Arduino terminal as well as on LEDs.
 * 
 * Pin and wiring connections:
 * RCWL-0516       Wemos D1 Mini
 * ---------       -------------
 * Vcc             5V         
 * OUT             D5   
 * GND             GND
 * Analog          A0
 * 
 * LEDs            Wemos D1 Mini         Meaning
 * --------        -------------         -----------------------------------------------------
 * Onboard         D4                    Onboard blue LED toggles with OUT pin on RCWL-0516
 * Green           D3                    RCWL-0516 output is just at or above noise level
 * Yellow          D2                    RCWL-9196 analog reading is low range
 * Orange          D1                    RCWL-9196 analog reading is medium range
 * Blue            D0                    RCWL-9196 analog reading is high range
 * Red             D7                    RCWL-9196 analog reading is very high range
 * 
 * Protect all LEDs with current limiting resistors
 *                  
 * Frank Milburn  May 2018
 */

const int radarPin = D5;
const int analogPin = A0;
const int outPin = LED_BUILTIN;
const int redPin = D7;
const int greenPin = D3;
const int yellowPin = D2;
const int orangePin = D1;
const int bluePin = D0;
boolean lastRadarState = 0;
unsigned long radarCount = 0;
 
void setup() {
  pinMode(radarPin, INPUT);
  digitalRead(radarPin);
  pinMode(outPin, INPUT);
  digitalWrite(outPin, LOW);
  pinMode (redPin, OUTPUT);
  digitalWrite(redPin, LOW);
  pinMode (greenPin, OUTPUT);
  digitalWrite(greenPin, LOW);
  pinMode (yellowPin, OUTPUT);
  digitalWrite(yellowPin, LOW);
  pinMode (orangePin, OUTPUT);
  digitalWrite(orangePin, LOW);
  pinMode (bluePin, OUTPUT);
  digitalWrite(bluePin, LOW);
  
  // delay(5000);                  // let things settle

  Serial.begin(115200);
  Serial.println("Starting RCWL-0516 Analog Test...");
}

void loop() {
  
  boolean radarTrap = false;
  radarTrap = digitalRead(radarPin);
  if (radarTrap == true){
    if (lastRadarState == false){
      radarCount++;
      lastRadarState = true;
      //Serial.print("Motion detected: ");
      //Serial.println(radarCount);
      digitalWrite(outPin, HIGH);
    }
  }
  else {
    lastRadarState = false;
    digitalWrite(outPin, LOW);
  }
  unsigned analogValue = abs(analogRead(analogPin) - 612);
  Serial.println(analogValue);
  if (analogValue > 25) {
    digitalWrite(greenPin, HIGH);
  }
  else {
    digitalWrite(greenPin, LOW);
  }
  if (analogValue > 50) {
    digitalWrite(yellowPin, HIGH);
  }
  else {
    digitalWrite(yellowPin, LOW);
  }
  if (analogValue > 100) {
    digitalWrite(orangePin, HIGH);
  }
  else {
    digitalWrite(orangePin, LOW);
  }
  if (analogValue > 200) {
    digitalWrite(bluePin, HIGH);
  }
  else {
    digitalWrite(bluePin, LOW);
  }
  if (analogValue > 250) {
    digitalWrite(redPin, HIGH);
  }
  else {
    digitalWrite(redPin, LOW);
  }
  delay(25);
}
