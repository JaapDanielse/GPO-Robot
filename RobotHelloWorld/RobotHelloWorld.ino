/*
  RobotHelloWorld

  V0.3   JpD & JnD 21-08-2016
         Distance measured during run. Sequence from loop.

  V0.2   JpD & JnD 21-08-2016
         Turn implemented, Slow and Fast implemented.

  V0.1   Jpd 21-08-2016
         Name changed to RobotHelloWorld. Stop using short reverse burst
*/


//-----------------------------------------------------------------------------------------
// Motor Direction Values

 #define STOP     0 //
 #define FORWARD  1 //
 #define REVERSE  2 //

// Speed and Motor PWM
 #define SLOW 250 //
 #define FAST 650 //

 #define SLOWPWM 80  //
 #define FASTPWM 255 //

 #define MAXLOOP 2 //

//-----------------------------------------------------------------------------------------
// Global Variables




//-----------------------------------------------------------------------------------------

void setup() 
{
  Serial.begin(115200); // set up Serial library at 115200 bps
  Serial.println("Robot Hello World!");
  speedSensorInit();
  distanceSensorInit();
  motorControlInit();

/*  
  Serial.println("Motor 1 Forward");
  motorControl(1, FORWARD, 255);
  delay(2000);
  motorControl(1, STOP, 0);
  delay(2000);
  Serial.println("Motor 1 Backward");
  motorControl(1, REVERSE, 255);
  delay(2000);
  motorControl(1, STOP, 255);

  delay(2000);
  Serial.println("Motor 2 Forward");
  motorControl(2, FORWARD, 255);
  delay(2000);
  motorControl(2, STOP, 0);
  delay(2000);
  Serial.println("Motor 2 Backward");
  motorControl(2, REVERSE, 255);
  delay(2000);
  motorControl(2, STOP, 0);

  while(1)
  {
   Serial.print( distanceSensorRead());
   Serial.println(" cm.");
   delay(250);
  }

*/



}


//-----------------------------------------------------------------------------------------

void loop() 
{

  static int action = 1;
  static int lastAction = 0;
  static int loopCount = 0;

  static int setDistance = 0;

  
  switch ( action )
  {
    case 1:
    {
      Serial.print("action: ");
      Serial.println( action );
      lastAction = 1;
      speedSensorClear();
      setDistance = 1000;
      action = 2;
      break;
    }

    case 2:
    {
      Serial.print("action: ");
      Serial.println( action );
      lastAction = action;
      if ( !driveForward( setDistance ,SLOW, 10, false ) ) 
      { 
        setDistance = setDistance - speedSensorReadDistance(1);
        action=10; 
        break; 
      }
      setDistance = 0;
      
      action = 4;
      break;
    }
    
    case 3:
    {
      Serial.print("action: ");
      Serial.println( action );
      lastAction = action;
      if ( !driveForward(300,SLOW, 10, false )) { action=10; break; }
      action = 4;
      break;
    }
    
    case 4:
    {
      Serial.print("action: ");
      Serial.println( action );
      lastAction = action;
      driveStop();
      delay(500);
      action = 5;
      break;
    }

    case 5:
    {
      Serial.print("action: ");
      Serial.println( action );
      lastAction = action;
      driveTurn(180);
      driveStop();
      loopCount++;
      action = 1;
      break;
    }

    
    case 10:
    {
      Serial.print("action: ");
      Serial.println( action );
      driveStop();
      delay(500);
      while(distanceSensorCheckObstacle(10));
      action = lastAction;
      break;
    }


    default:
      break;
  }

  if (loopCount >= MAXLOOP) while(1);
  



  
  
  
}
