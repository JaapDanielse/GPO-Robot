/*
  SpeedSensor Module
  
  Using IR led and photo diode to sense slots in the encoder disk.
  The speedSensorInit routine sets up the interupt mechanism for the chosen pin's
  The connected interrupt service routine ISR(PCINT1_vect) catches the gate's interrupt 
  and determines the pin causing the interrupt.
  
*/

//-----------------------------------------------------------------------------------------------
// speedSensor pin declarations

  #define SPEEDSENSOR1 18 // Speed sensor 1 pin (motor 1)
  #define SPEEDSENSOR2 19 // Speed sensor 2 pin (motor 2)


//-----------------------------------------------------------------------------------------------
// speedSensor global variables

  int speedSensor1Count = 0; 
  int speedSensor2Count = 0;

  int speedSensor1Speed = 0; 
  int speedSensor2Speed = 0; 

  boolean speedSensor1State = false;
  boolean speedSensor2State = false;

  unsigned long speedSensor1Time = 0; 
  unsigned long speedSensor2Time = 0; 

  char speedSensor1Direction = 1;
  char speedSensor2Direction = 1;


//-----------------------------------------------------------------------------------------------
void speedSensorInit()
{
  pinMode(SPEEDSENSOR1, INPUT); // Speedsensor 1 pin as input
  pinMode(SPEEDSENSOR2, INPUT); // Speedsensor 2 pin as input

  // setup Pin Change Interrupt for AD pin 4 en 5
  PCIFR = (1<<PCIF1);  // Pin Change Interrupt - Clear Interrupt C-port = AD-port
  PCICR = (1<<PCIE1);  // Pin Change Interrupt - Enable C-port = AD-port
  PCMSK1 = (1<<PCINT12) | (1<<PCINT13);	// Pin Change Interrupt - Enable AD4 (pin 18) & AD5 (pin 19) pins 

  speedSensor1State = digitalRead(SPEEDSENSOR1); // get initial value
  speedSensor2State = digitalRead(SPEEDSENSOR2); // get initial value

} 


//-----------------------------------------------------------------------------------------------
ISR(PCINT1_vect) // handle interrupt for A0 to A5
{ // Speed sensor interrupt service routine (keep it short!)
  
  if (digitalRead(SPEEDSENSOR1) != speedSensor1State) // sensor 1 changed?
  {
    speedSensor1State = !speedSensor1State; // store current value (HIGH/LOW)
    if(speedSensor1State) // only react on becomming high (start of sensor slot)
    {
      speedSensor1Count += speedSensor1Direction; // count the slots and direction +1/-1
      speedSensor1Speed = (float(1000.0/(millis()-speedSensor1Time))*11); // calculate speed in mm/s (one slot =  11 mm. movement)
      speedSensor1Time = millis(); // setup for next measurement
      driveSpeedSensorCallback( 1, speedSensor1Speed, speedSensor2Speed ); // do callback
    }
  }

  if (digitalRead(SPEEDSENSOR2) != speedSensor2State) // sensor 2 changed?
  {
    speedSensor2State = !speedSensor2State; // store current value (HIGH/LOW)
    if(speedSensor2State) // only react on becomming high (start of sensor slot)
    {
      speedSensor2Count += speedSensor2Direction; // count the slots and direction +1/-1
      speedSensor2Speed = (float(1000.0/(millis()-speedSensor2Time))*11); // calculate speed in mm/s (one slot =  11 mm. movement)
      speedSensor2Time = millis(); // setup for next measurement
      driveSpeedSensorCallback( 2, speedSensor1Speed, speedSensor2Speed ); // do callback
    }
  } 
}


//-----------------------------------------------------------------------------------------------
void speedSensorSetDirection( byte sensorId, byte sensorDirection )
{ // set the direction for the specified sensor
  
  if (sensorId == 1) // sensor 1 / motor 1
  {
    if (sensorDirection == FORWARD) speedSensor1Direction=1; // set for forward distance count
    if (sensorDirection == REVERSE) speedSensor1Direction=-1; // set for reverse distance count
  }
  if (sensorId == 2) // sensor 2 / motor 2
  {
    if (sensorDirection == FORWARD) speedSensor2Direction=1; // set for forward distance count
    if (sensorDirection == REVERSE) speedSensor2Direction=-1; // set for reverse distance count
  }
}

//-----------------------------------------------------------------------------------------------
byte speedSensorGetDirection( byte sensorId )
{ // return the set direction for the requested sensor
  
  if (sensorId == 1) // sensor 1 / motor 1
  {
    if (speedSensor1Direction == 1) 
      return FORWARD;
    else
      return REVERSE;
  }

  if (sensorId == 2) // sensor 2 / motor 2
  {
    if (speedSensor2Direction == 1) 
      return FORWARD;
    else
      return REVERSE;
  }

}

//-----------------------------------------------------------------------------------------------
int speedSensorClear()
{ // clear speedsensor counts
  
    speedSensor1Count = 0;
    speedSensor2Count = 0;

}

//-----------------------------------------------------------------------------------------------
int speedSensorReadCount( byte sensorId )
{ // return the requested sensor interrupt count
  
  if (sensorId == 1)
    return (speedSensor1Count);
  if (sensorId == 2)
    return (speedSensor2Count);
}

//-----------------------------------------------------------------------------------------------
int speedSensorReadTime( byte sensorId  )
{ // return the time between the last measurement and now
  
  if (sensorId == 1)
    return (millis()-speedSensor1Time); // return ms passed since previous interrupt

  if (sensorId == 2)
    return (millis()-speedSensor2Time); // return ms passed since previous interrupt
}


// end module
