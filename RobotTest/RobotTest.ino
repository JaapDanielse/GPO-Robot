/*
  RobotTest
  Hardware test voor de GPO-DevLab Autonomous Robot

  V1.0    JpD 20-08-2016 
          Original version
*/


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
  Serial.println("Checkin speed sensor connection");
  delay(1000);
  if (speedSensorReadCount(1) > 5)
  {
     Serial.println("Speedsensor 1 - probably not connected");
     Serial.println("Switch off, disconnect USB and check connections");
     while(1);
  }
  if (speedSensorReadCount(2) > 5)
  {
     Serial.println("Speedsensor 2 - probably not connected");
     Serial.println("Switch off, disconnect USB and check connections");
     while(1);
  }
  Serial.println("Speed sensor connection seems OK");

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
     Serial.println("Yes - Checking Speedsensor 1");
  if (speedSensorReadCount(1) > 10)
     Serial.println("Speedsensor 1 - OK");
  if (speedSensorReadCount(2) > 10)
     Serial.println("Speedsensor 2 - detected change speedsensor connection and try again");

  Serial.println("");
  Serial.println("- Motor 1 Reverse - slow");
  speedSensorClear();
  motorControl(1, REVERSE, 120);
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
     Serial.println("Yes - Checking Speedsensor 2");
  if (speedSensorReadCount(2) > 10)
     Serial.println("Speedsensor 2 - OK");
  if (speedSensorReadCount(1) > 10)
     Serial.println("Speedsensor 1 - detected change speedsensor connection and try again");

  Serial.println("");
  Serial.println("- Motor 2 Reverse - slow");
  speedSensorClear();
  motorControl(2, REVERSE, 120);
  delay(2000);
  motorControl(2, STOP, 0);
  if (speedSensorReadCount(1) > 10)
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

  for(int i=0; i<20; i++)
  {
    Serial.print( distanceSensorRead());
    Serial.println(" cm.");
    if(distanceSensorRead() > 0) sensorOk = true;   
    delay(250);
  }

  if (sensorOk)
  {
     Serial.println("Distance sensor OK");
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

