/* 
 * HC-SR04 Ultrasonic Distance Sensor Example
 * Demonstrates sensing distance with the HC-SR04 using Wemos D1 Mini
 *
 * Note: The ESP8266 is not 5V tolerant but the HC-SR04 requires 5V!
 * Logic level conversioin (e.g. voltage divider) must be used on
 * the output pin (Echo) of the HC-SR04.
 *                
 * Frank Milburn April 2018

 * Pin and wiring connections:
 * HC-SR04   Wemos D1 Mini
 * -------   -------------
 * Vcc       5V
 * Trig      D3                     
 * Echo      D2   !!Voltage Divider Required!!
 * GND       GND

 * Voltage Divider
 * HC-SR04                Wemos D1 Mini
 * Echo -- 10k Ohm --+--  D2     
 *                   |
 *                15k Ohm
 *                   |
 *                  GND
 *                  
 * Frank Milburn  May 2018
 */

const int trigPin = D3;
const int echoPin = D2;

void setup()
{
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);      
   
  Serial.begin(115200);
  Serial.println("Starting HC-SR04 Test...");  
}

void loop()
{
  unsigned long pulseLength;
  unsigned int inches;
  unsigned int centimeters;
  
  digitalWrite(trigPin, LOW);            // send low to get a clean pulse
  delayMicroseconds(5);                  // let it settle
  digitalWrite(trigPin, HIGH);           // send high to trigger device
  delayMicroseconds(10);                 // let it settle

  // We time how long the sound takes to go out and return...
  pulseLength = pulseIn(echoPin, HIGH);  // measure pulse coming back

  // Sound travels about 1130 feet/sec at sea level and normal conditions
  // or 13,560 inches/sec.  The pulseIn function is in micro seconds
  // so the speed would then be 0.01356 in / ms.
  
  // Use float if the precision is needed but this example will use integer
  // math for speed.  Then, 1/.001356 = 73.75,  or approx. 74 ms/in.  Since
  // the signal is travelling out and back, we want half the time for 
  // distance.  Therefore divide by 2 and then 74 ms/in (which is divide
  // by 148).  Want centimeters?  The devisor is 58.
  
  inches = pulseLength / 148; 
  centimeters = pulseLength / 58;  
                                    
  Serial.print("Distance = ");
  Serial.print(inches);
  Serial.print(" inches");
  Serial.print("       ");
  Serial.print(centimeters);
  Serial.println(" centimeters");
  delay(1000);
}
