/*
 
 Drive Module
 
 The drive math:
 The speed sensor wheels have 20 slots and 20 spokes.
 Since all changes in the sensor pins result in an interrupt 
 there will be 40 interrupts for a full rotation of a wheel
 but since the slots have a different size than the spokes
 we use only the interupts for the start of a slot (HIGH).
 
 Te wheel circumference is 220 mm. 
 One interrupt is therfore equal to 220/20 = 11 mm. movement.
 
 When turning the distance between the wheel centers is 140 mm.
 If only one wheel runs a 90 deg. turn equals:
(2*pi*140)/4 = 220 (rounded) = one full wheel turn. (/4 = 90 deg turn)

 If both wheels are running the center of the circle will be in the middle.
 The radius will then be 70 mm. So (2*pi*70)/4 = 110 mm. = 10 interrupts.
 The other wheel will be turning the other way also 10 interrupts.
 A difference of 20 interrupts therfore means a 90 deg. direction change.
 I guess this also applies when the robot does a curve.
 
 The forward motion is calulated using the average interrupt count of the motors
 times 11 mm.
 
 
*/

//-----------------------------------------------------------------------------------------------
// speedSensor global variables

  boolean driveActive = false;
  
  int driveMotorSpeed;
  int driveMotorPwm = FASTPWM;


//-----------------------------------------------------------------------------------------------
boolean driveForward( int distance, int driveSpeed, int obstacleDistWarning, boolean clear )
{
  int stallTime;
  int obstacleDist;
  
  stallTime = (1000/(float (driveSpeed/11.0)))*2.0; //caclulate the time in ms required for one slot at this speed times 1.5

  if (clear) speedSensorClear();
  distance = distance + speedSensorReadDistance(1);
  driveMotorSpeed = driveSpeed;
  if (driveSpeed == SLOW) 
    driveMotorPwm = SLOWPWM;
  else 
    driveMotorPwm = FASTPWM;
  
  speedSensorSetDirection(1, FORWARD); // set direction for the sensors
  speedSensorSetDirection(2, FORWARD); // set direction for the sensors

  driveActive = true;
  motorControl(1, FORWARD, 255); // output to motor
  motorControl(2, FORWARD, 255); // output to motor

  Serial.print("start stalltime: ");
  Serial.print(stallTime);
  Serial.print(" speed: ");
  Serial.print(driveMotorSpeed);
  Serial.print(" dist: ");
  Serial.print(distance);
  Serial.println();


  while( speedSensorReadDistance(1) < distance ) 
  {
    delay(stallTime);
    driveCatchStall(stallTime);
    if ( distanceSensorCheckObstacle(obstacleDistWarning) ) return false;
  }

  driveActive = false;
  Serial.print(speedSensorReadDistance(1));
  Serial.println("stop");

  return true;
  
}


//-----------------------------------------------------------------------------------------------
void driveTurn(int turnDegrees)
{

  int driveSpeed = SLOW;
  int distance;
  int stallTime;

  distance = int (1.2 * float(turnDegrees));
  
  
  stallTime = (1000/(float (driveSpeed/11.0)))*2.0; //caclulate the time in ms required for one slot at this speed times 1.5

  speedSensorClear();
  driveMotorPwm = SLOWPWM;
  driveMotorSpeed = SLOW;
  speedSensorSetDirection(1, FORWARD); // set direction for the sensors
  speedSensorSetDirection(2, REVERSE); // set direction for the sensors
  driveActive = true;
  motorControl(1, FORWARD, 255); // output to motor
  motorControl(2, REVERSE, 255); // output to motor
  
  while( speedSensorReadDistance(1) < distance ) 
  { 
    delay(stallTime);
    driveCatchStall(stallTime);
  }
  driveActive = false;
}


//-----------------------------------------------------------------------------------------------
void driveStop()
{
  driveActive = false;

  if ( speedSensorGetDirection(1)==FORWARD ) // check direction and revers
    motorControl(1, REVERSE, 80); // output to motor
  else
    motorControl(1, FORWARD, 80); // output to motor
  
  if ( speedSensorGetDirection(2)==FORWARD ) // check direction and reverse
    motorControl(2, REVERSE, 80); // output to motor
  else
    motorControl(2, FORWARD, 80); // output to motor
  
  delay(100); // wait for stop

  motorControl(1, speedSensorGetDirection(1), 0); // reset to original direction
  motorControl(2, speedSensorGetDirection(2), 0); // reset to original direction
}


//-----------------------------------------------------------------------------------------------
void driveSpeedSensorCallback( int Sensor1Speed, int Sensor2Speed, int Sensor1Count, int Sensor2Count )
{
  // extends interrupt service routine: keep it short!

  Serial.print(Sensor1Speed);
  Serial.print(" ");
  Serial.print(Sensor2Speed);
  Serial.print(" ");
  Serial.print(Sensor1Count);
  Serial.print(" ");
  Serial.print(Sensor2Count);
  Serial.print(" ");
  Serial.print(speedSensorReadDistance(1));
    
  if (driveActive)
  {
    if ( Sensor1Speed < driveMotorSpeed && abs(Sensor1Count) <= abs(Sensor2Count) )
    {
      motorSpeed(1, driveMotorPwm); // too slow: engine on
      Serial.print(" 1-on");
    }
    else
    {
      motorSpeed(1, 0); // too fast: engine off
      Serial.print(" 1-off");
    }

    if ( Sensor2Speed < driveMotorSpeed && abs(Sensor2Count) <= abs(Sensor1Count) )
    {
      motorSpeed(2, driveMotorPwm); // store the initial drive speed
      Serial.print(" 2-on");
    }
    else
    {
      motorSpeed(2, 0); // store the initial drive speed
      Serial.print(" 2-off");
    }
  }
 
  Serial.println();
}


//-----------------------------------------------------------------------------------------------
void driveCatchStall(int stallTime)
{
    if (speedSensorReadTime(1) > stallTime) // have we stalled?
    {
      motorSpeed(1, FASTPWM); // yes!: start moving again
      Serial.print(" stall 1 ");
    }
    
    if (speedSensorReadTime(2) > stallTime) // have we stalled?
    {
      motorSpeed(2, FASTPWM); // yes!: start moving again
      Serial.print(" stall 2 ");
    }
}


// End Module
