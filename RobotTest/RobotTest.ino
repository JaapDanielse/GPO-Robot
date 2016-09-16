/*
  RobotTest
  Hardware test voor de GPO-DevLab Autonomous Robot

  V1.1    JpD 20-08-2016 
          Improved loose contact detection, halt on sensor lead error.

  V1.0    JpD 20-08-2016 
          Original version
*/


#define DISTANCESENSEGND 14 // distance sensor echo pin
#define DISTANCESENSECHO 15 // distance sensor echo pin
#define DISTANCESENSTRIG 16 // distance sensor trigger pin
#define DISTANCESENSEVCC 17 // distance sensor echo pin

#define SPEEDSENSOR1 18 // Speed sensor 1 pin (motor 1)
#define SPEEDSENSOR2 19 // Speed sensor 2 pin (motor 2)


//-----------------------------------------------------------------------------------------
// Motor Direction Values

#define STOP     0 //
#define FORWARD  1 //
#define REVERSE  2 //




//-----------------------------------------------------------------------------------------
// Global Variables




//-----------------------------------------------------------------------------------------

void setup() 
{
  boolean sensorOk = false;
  
  speedSensorInit();
  distanceSensorInit();
  motorControlInit();

  Serial.begin(9600); // set up Serial library at 115200 bps
  Serial.println("RobotTest");
  Serial.println("Checking GPO-Devlab Autonomous Robot hardware");
  Serial.println("");
  Serial.println("Place the robot from the ground so the wheels can turn freely. And switch on battery power.");
  Serial.println("Ready? [y/n]");
  while (!getReply());



  Serial.println("");
  Serial.println("Checking loose connections");

  sensorOk = true;
  if ( analogRead(DISTANCESENSECHO) > 50 )
  {
     Serial.println(analogRead(DISTANCESENSECHO));
     Serial.println("Ultrasonic sensor - probably not connected");
     sensorOk = false;
  }
  if ( analogRead(SPEEDSENSOR1) > 50 && analogRead(SPEEDSENSOR1) < 975 )
  {
     Serial.println(analogRead(SPEEDSENSOR1));
     Serial.println("Speedsensor 1 - probably not connected");
     sensorOk = false;
  }
  if ( analogRead(SPEEDSENSOR2) > 50 && analogRead(SPEEDSENSOR2) < 975 )
  {
     Serial.println(analogRead(SPEEDSENSOR2));
     Serial.println("Speedsensor 2 - probably not connected");
     sensorOk = false;
 }
 if (!sensorOk)
 {
    Serial.println("Check sensor connections");
    while(1);
 }
 
  if (speedSensorReadCount(1) > 0)
  {
     Serial.println("Speedsensor 1 - probably not connected");
     Serial.println("Switch off, disconnect USB and check connections");
     while(1);
  }
  if (speedSensorReadCount(2) > 0)
  {
     Serial.println("Speedsensor 2 - probably not connected");
     Serial.println("Switch off, disconnect USB and check connections");
     while(1);
  }
  Serial.println("Connections probably ok");

  Serial.println("");
  Serial.println("Testing motor 1");
  Serial.println("- Motor 1 Forward");

  speedSensorClear();
  motorControl(1, FORWARD, 255);
  delay(2000);
  motorControl(1, STOP, 0);

  Serial.println("Did motor 1 (left) move forward? [y/n]");
  if (!getReply())
  {
     Serial.println("No - Switch off, disconnect USB and check connections");
     while(1);
  }
  else
  {
     Serial.print("S1: ");
     Serial.print(speedSensorReadCount(1));
     Serial.print(" S2: ");
     Serial.print(speedSensorReadCount(2));
     Serial.println();
     
     Serial.println("Yes - Checking Speedsensor 1");
     if (speedSensorReadCount(1) > 10)
       Serial.println("Speedsensor 1 - OK");
     else
     {
       Serial.println("Speedsensor 1 - Not read - check connections");
       if (speedSensorReadCount(2) > 10)
         Serial.println("Speedsensor 2 - Read: sensor 2 connected to motor 1 ");
       while(1);
     }
     if (speedSensorReadCount(2) > 10)
       Serial.println("Speedsensor 2 - Read: short circuit between sensors");
 }
 
  Serial.println("");
  Serial.println("- Motor 1 Reverse - slow");
  speedSensorClear();
  motorControl(1, REVERSE, 150);
  delay(2000);
  motorControl(1, STOP, 0);
  if (speedSensorReadCount(1) > 10)
  {
     Serial.println("Speedsensor 1 - showed no movement: motor shield error");
     while(1);
  }
  Serial.println("Did motor 1 (left) move backward? [y/n]");
  if (!getReply())
  {
     Serial.println("No:  Motor shield error");
     while(1);
  }
  
   
  Serial.println("");
  Serial.println("Testing motor 2");
  Serial.println("- Motor 2 Forward");

  speedSensorClear();
  motorControl(2, FORWARD, 255);
  delay(2000);
  motorControl(2, STOP, 0);

  Serial.println("Did motor 2 (right) move forward? [y/n]");
  if (!getReply())
  {
     Serial.println("No - Switch off, disconnect USB and check connections");
     while(1);
  }
  else
  {
     Serial.println("Yes - Checking Speedsensor 2");

    if (speedSensorReadCount(2) > 10)
       Serial.println("Speedsensor 2 - OK");
     else
     {
       Serial.println("Speedsensor 2 - Not read - check connections");
       if (speedSensorReadCount(1) > 10)
         Serial.println("Speedsensor 1 - Read: sensor 1 connected to motor 2 ");
       while(1);
     }
     if (speedSensorReadCount(1) > 10)
       Serial.println("Speedsensor 1 - Read: short circuit between sensors");
  }
  
  Serial.println("");
  Serial.println("- Motor 2 Reverse - slow");
  speedSensorClear();
  motorControl(2, REVERSE, 150);
  delay(2000);
  motorControl(2, STOP, 0);
  if (speedSensorReadCount(2) > 10)
  {
     Serial.println("Speedsensor 2 - showed no movement: motor shield error");
     while(1);
  }
  Serial.println("Did motor 2 (Right) move backward? [y/n]");
  if (!getReply())
  {
     Serial.println("No:  Motor shield error");
     while(1);
  }

     
  Serial.println("");
  Serial.println("Testing Distance Sensor");
  Serial.println("Hold your hand in front of the sensor");
  Serial.println("Ready? [y/n]");
  while (!getReply());

  sensorOk = false;
  for(int i=0; i<20; i++)
  { 
    int dist;

    dist = distanceSensorRead();   

    Serial.print( dist);
    Serial.println(" cm.");
    
    if(dist > 0 && dist < 40) sensorOk = true;   
    delay(250);
  }

  if (sensorOk)
  {
     Serial.println("Distance sensor reading OK");
     Serial.println("Your robot seems to be working");
     while(1);     
  }
  else
  {
     Serial.println("Distance sensor FAIL");
     Serial.println("Switch off, disconnect USB and check connections");
     while(1);
  }

}

//-----------------------------------------------------------------------------------------
void loop() 
{
  
  
  
}

//-----------------------------------------------------------------------------------------
boolean getReply()
{  

  char AnswerChar;

  while(Serial.available()) Serial.read(); //clear buffer

  while(!Serial.available()); // wait for reply
  while(1)
  {
    while(Serial.available())
    {
      AnswerChar = Serial.read();
      if (AnswerChar == 'y' or AnswerChar == 'Y') return true;
      if (AnswerChar == 'n' or AnswerChar == 'N') return false;
    }
  }
}

