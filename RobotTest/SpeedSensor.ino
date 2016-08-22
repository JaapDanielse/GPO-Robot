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

  boolean speedSensor1State = false;
  boolean speedSensor2State = false;


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
{
  // interrupt service routine (keep it short!)
  
  if (digitalRead(SPEEDSENSOR1) != speedSensor1State) // sensor 1 changed?
  {
    speedSensor1State = !speedSensor1State; // store current value (HIGH/LOW)
    if(speedSensor1State) // only react on becomming high (start of sensor slot)
    {
      speedSensor1Count++;
    }
  }

  if (digitalRead(SPEEDSENSOR2) != speedSensor2State) // sensor 2 changed?
  {
    speedSensor2State = !speedSensor2State; // store current value (HIGH/LOW)
    if(speedSensor2State) // only react on becomming high (start of sensor slot)
    {
      speedSensor2Count++;
    }
  } 
}


//-----------------------------------------------------------------------------------------------
int speedSensorReadCount( byte sensorId )
{
  if (sensorId == 1)
    return (speedSensor1Count);
  if (sensorId == 2)
    return (speedSensor2Count);
}

//-----------------------------------------------------------------------------------------------
int speedSensorClear()
{
    speedSensor1Count = 0;
    speedSensor2Count = 0;
}

// end module
