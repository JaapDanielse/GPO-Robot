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

 Calculating from degrees to distance of one wheel (both wheels turning oposite)
 90 deg = 110 mm. So 110/90 = 1.2 mm / degree (minimum turn will be 10 deg.)
  
 The forward motion is calulated using the average interrupt count of the motors
 times 11 mm.
 
 
*/

//-----------------------------------------------------------------------------------------------
// drive status value
#define IDLING   0 // 
#define STOPPED  1 // 
#define DRIVING  3 //
#define TURNING  4 //

// drive global variables
byte driveStatus = STOPPED;  
int driveMotorSpeed = 0; // Speed in cm/sec 
int driveMotorPwm = 0; // pwm value 
int driveSensor1Count = 0; // speed sensor 1 interrupt counter for action
int driveSensor2Count = 0; // speed sensor 2 interrupt counter for action
int driveSensorDiff = 0; // stop error for next action
int driveStallTime = 0; // time

//-----------------------------------------------------------------------------------------------
boolean driveStraight(byte direction, int distance, int driveSpeed, byte obstacleDistWarning )
{ // drive in a straight line for a given distance with a given speed. Watch for obstacles closer then given distance
  
  driveStallTime = (1000/(float (driveSpeed/11.0)))*1.5; //caclulate the time in ms required for one slot at this speed times 1.5

  driveMotorSpeed = driveSpeed; // storeDriveSpeed

  if (driveSpeed == SLOW) // determine PWM value
    driveMotorPwm = SLOWPWM; 
  else  
    driveMotorPwm = FASTPWM;

  driveSensor2Count = 0; // clear speed sensor interrupt Count
  driveSensor1Count = 0; // clear speed sensor interrupt Count

  // if we stopped at an (unintended) angle correct for this.
  if ( driveSensorDiff >= 0 ) // we should have driven straight = 0; positive = 1 too fast negative = 2 too fast
    driveSensor1Count = abs(driveSensorDiff);
  else
    driveSensor2Count = abs(driveSensorDiff);

  driveStatus = DRIVING; // we are driving

  speedSensorSetDirection(1, direction); // set direction for the sensors
  speedSensorSetDirection(2, direction); // set direction for the sensors

  motorControl(1, direction, FASTPWM); // output to motor
  motorControl(2, direction, FASTPWM); // output to motor
  
  while( driveDistanceDone() < distance ) // until distance covered
  {
    delay(driveStallTime/3); // wait a bit
    driveCatchStall(); // catch stall 
    if ( distanceSensorCheckObstacle(obstacleDistWarning) ) // check for obstacles
    { // an obstacle detected within range 
      driveStatus = IDLING; // no longer driving
      return false; // signal obstacle detected
    }
  }

  driveStatus = IDLING; // no longer driving
  return true; // normal exit
  
}


//-----------------------------------------------------------------------------------------------
void driveTurn(int turnDegrees)
{ // make a turn for a (approximate) given number of degrees

  int driveSpeed = SLOW;
  int distance; 

  distance = int (1.2 * float(turnDegrees)); // distance for one wheel: 1.2 mm per degree 

  driveStallTime = (1000/(float (driveSpeed/11.0)))*1.5; //caclulate the time in ms required for one slot at this speed times 1.5

  driveSensor1Count = 0; // clear speed sensor interrupt Count
  driveSensor2Count = 0; // clear speed sensor interrupt Count

  driveMotorSpeed = SLOW; // set slow speed
  driveMotorPwm = SLOWPWM; // set slow pwm
  driveStatus = TURNING; // we are turning

  if (turnDegrees >= 0)
  { // right turn
    speedSensorSetDirection(1, FORWARD); // set direction for the sensors
    speedSensorSetDirection(2, REVERSE); // set direction for the sensors
    motorControl(1, FORWARD, FASTPWM); // start fast, output to motor
    motorControl(2, REVERSE, FASTPWM); // start fast output to motor
  }
  else
  { // left turn
    speedSensorSetDirection(1, REVERSE); // set direction for the sensors
    speedSensorSetDirection(2, FORWARD); // set direction for the sensors
    motorControl(1, REVERSE, FASTPWM); // output to motor
    motorControl(2, FORWARD, FASTPWM); // output to motor
  }
    
  while( driveSensor1Count * 11 < distance ) // do while stop not complete
  { 
    delay(driveStallTime/3); // wait a bit
    driveCatchStall(); // catch stall
  }
  driveStatus = IDLING; // no longer turning
  
  return false;
  
}


//-----------------------------------------------------------------------------------------------
void driveStop()
{
  if ( speedSensorGetDirection(1)==FORWARD ) // check direction and revers
    motorControl(1, REVERSE, SLOWPWM); // output to motor
  else
    motorControl(1, FORWARD, SLOWPWM); // output to motor
  
  if ( speedSensorGetDirection(2)==FORWARD ) // check direction and reverse
    motorControl(2, REVERSE, SLOWPWM); // output to motor
  else
    motorControl(2, FORWARD, SLOWPWM); // output to motor
  
  delay(80); // wait for stop

  motorControl(1, speedSensorGetDirection(1), 0); // reset to original direction
  motorControl(2, speedSensorGetDirection(2), 0); // reset to original direction

  driveStatus = STOPPED; // we hav stopped
  
}


//-----------------------------------------------------------------------------------------------
int driveDistanceDone()
{ // returns the distacne driven 
  
  return ((( driveSensor1Count + driveSensor2Count)/2) * 11) ;

}


//-----------------------------------------------------------------------------------------------
void driveSpeedSensorCallback(byte SensorId, int Sensor1Speed, int Sensor2Speed )
{ // called on interrupt of the speed sensors. Switches motors on or off depending on speed and other wheel interrupt count
  // extends interrupt service routine: keep it short!

  if (SensorId == 1) 
    driveSensor1Count++; // keep track of this action
  else // SensorId == 2
    driveSensor2Count++; // keep track of this action

  driveSensorDiff = abs(driveSensor1Count) - abs(driveSensor2Count); // store difference

  Serial.print(Sensor1Speed);
  Serial.print(" ");
  Serial.print(Sensor2Speed);
  Serial.print(" ");
  Serial.print(driveSensor1Count);
  Serial.print(" ");
  Serial.print(driveSensor2Count);
  Serial.print(" ");
  Serial.print(driveDistanceDone());

  if (driveStatus == DRIVING || driveStatus == TURNING)
  { 
    if ( Sensor1Speed < driveMotorSpeed && abs(driveSensor1Count) <= abs(driveSensor2Count) )
    { // we are on speed or slow and not ahead of motor 2
      motorSpeed(1, driveMotorPwm); // too slow: engine on
      Serial.print(" 1-on");
    }
    else
    { // we are too fast or ahead of motor 2
      motorSpeed(1, 0); // too fast: engine off
      Serial.print(" 1-off");
    }

    if ( Sensor2Speed < driveMotorSpeed && abs(driveSensor2Count) <= abs(driveSensor1Count) )
    { // we are on speed or slow and not ahead of motor 1
      motorSpeed(2, driveMotorPwm); // store the initial drive speed
      Serial.print(" 2-on");
    }
    else
    { // we are too fast or ahead of motor 1
      motorSpeed(2, 0); // store the initial drive speed
      Serial.print(" 2-off");
    }
  }
 
  Serial.println();
}


//-----------------------------------------------------------------------------------------------
void driveCatchStall()
{ // called from driveStraight and driveTurn to catch a stalling engine start it again.
  
    if ( speedSensorReadTime(1) > driveStallTime ) // have we stalled?
    {
      motorSpeed(1, FASTPWM); // yes: start moving again
      Serial.print(" stall 1 ");
    }
    
    if ( speedSensorReadTime(2) > driveStallTime ) // have we stalled?
    {
      motorSpeed(2, FASTPWM); // yes: start moving again
      Serial.print(" stall 2 ");
    }
}


// End Module
