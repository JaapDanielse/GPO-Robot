/*
 
 DistanceSensor Module
 
 Ultrasonic HC-SR04 distance sensor
 Uses a trigger pulse to send a ultrasone burst.
 Then waits for the echo to return 
 The duration divided by 2 and the result devided by 29.14 gives the distance in cm.
 (29.14 = time in microsecconds it takes sounds to travel a distance of 1cm)
 
*/
 
#define DISTANCESENSEGND 14 // distance sensor echo pin
#define DISTANCESENSECHO 15 // distance sensor echo pin
#define DISTANCESENSTRIG 16 // distance sensor trigger pin
#define DISTANCESENSEVCC 17 // distance sensor echo pin
 
void distanceSensorInit() 
{
  pinMode(DISTANCESENSEVCC, OUTPUT); // set trigger pin as output
  pinMode(DISTANCESENSEGND, OUTPUT); // set trigger pin as output
  pinMode(DISTANCESENSTRIG, OUTPUT); // set trigger pin as output
  pinMode(DISTANCESENSECHO, INPUT);  // set echo pin as input

  digitalWrite(DISTANCESENSEGND, LOW); // set ground pin low (misuse of data pin)  
  digitalWrite(DISTANCESENSTRIG, LOW); // set trigger pin low (trigger is high)  
  digitalWrite(DISTANCESENSEVCC, HIGH); // set vcc (power) pin high (misuse of data pin)  
}
 
int distanceSensorRead() 
{
  long duration; 
   
  digitalWrite(DISTANCESENSTRIG, HIGH); // trigger pin high
  delayMicroseconds(10); // 10 microseccond  delay
  digitalWrite(DISTANCESENSTRIG, LOW); // trigger pin low 
  duration = pulseIn(DISTANCESENSECHO, HIGH, 11656); // measure the echo time in microsecconds; limit to approx 200 cm

  return int (float(duration / 2) / 29.14); // calculate distance in cm
  
}
