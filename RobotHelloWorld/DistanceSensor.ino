/*
 
 DistanceSensor Module
 
 Ultrasonic HC-SR04 distance sensor
 Uses a trigger pulse to send a ultrasone burst.
 Then waits for the echo to return 
 The duration divided by 2 and the result devided by 29.14 gives the distance in cm.
 (29.14 = time in microsecconds it takes sounds to travel a distance of 1cm)

 After the measurement we need some time for echo's to decay.
 If a next measurement is requested within 10 ms. the previous value is returned
 
 
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

  for(int i=0; i < 100; i++) // warm up? the distance sensor
  {
    distanceSensorRead();
  }
}
 
//-----------------------------------------------------------------------------------------------
int distanceSensorRead() 
{
  #define ECHODELAY 10 //
  
  static long measureTime = 0;
  static int lastReturnValue = 0;
  long duration = 0;
  
  if( (millis()-measureTime) > ECHODELAY ) // if not to soon after the previous measurment to avoid echos from before
  {
    measureTime = millis(); // note the measurment time
    digitalWrite(DISTANCESENSTRIG, HIGH); // trigger pin high
    delayMicroseconds(10); // 10 microseccond  delay
    digitalWrite(DISTANCESENSTRIG, LOW); // trigger pin low 
    duration = pulseIn(DISTANCESENSECHO, HIGH, 12000); // measure the echo time in microsecconds; limit to approx 200 cm
    lastReturnValue = int (float(duration / 2) / 29.14); // calculate distance in cm
  }
  return lastReturnValue; // return the measurement (or the previous one if requested too soon )
  
}

//-----------------------------------------------------------------------------------------------
boolean distanceSensorCheckObstacle(int obstacleDistWarning)
{
   int distanceMeasured;
   
   distanceMeasured = distanceSensorRead();
   if( distanceMeasured > obstacleDistWarning || distanceMeasured == 0 )
     return false;

   Serial.print( "Obstacle: ");
   Serial.print( distanceMeasured );
   Serial.println(" cm.");
   return true;
}

