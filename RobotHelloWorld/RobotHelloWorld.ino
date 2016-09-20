/*
  RobotHelloWorld

  V0.9b  JpD 29-08-2016
         Removed neopixel code
  
  V0.9a  JpD 29-08-2016
         Corrected negative turn
  
  V0.8   JpD & TmD 29-08-2016
         Callback via pointer
  
  V0.7   JpD 28-08-2016
         More comments added
  
  V0.6   JpD 27-08-2016
         More Cleaning, comments added
  
  V0.5   JpD 27-08-2016
         Cleaned, corrections for errors when stopping
  
  V0.4   JpD & JnD 21-08-2016
         Distance sensor corrections.

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

// Speed and Motor PWM Slow
 #define SLOW 250 // Speed in mm/sec for slow drive
 #define SLOWPWM 80 // PWM value for slow drive (experiment with setting for your robot)

// Speed and Motor PWM Fast
 #define FAST 650 // Speed in mm/sec for fast drive
 #define FASTPWM 255 // PWM value for fast drive ( max pwm )

 #define MAXLOOP 4 // maximum time back and forth


//-----------------------------------------------------------------------------------------
// Typedefs
 typedef void (*ISRCallBack)( byte, int, int ); // callback declaration


//-----------------------------------------------------------------------------------------
void setup() 
{
  Serial.begin(115200); // set up Serial library at 115200 bps
  Serial.println("Robot Hello World!");
  
  speedSensorInit( driveSpeedSensorCallback );
  distanceSensorInit();
  motorControlInit();
  
}


//-----------------------------------------------------------------------------------------
void loop() 
{

  static int action = 1;
  static int lastAction = 0; 
  static int loopCount = 0;
  static int setDistance = 0;

  int i=0;
 
  
  switch ( action )
  {
    case 1:
    { // init for drive
      Serial.print("action: ");
      Serial.println( action );
      lastAction = action;
      speedSensorClear(); // clear counts
      setDistance = 700; // set distance for drive in mm.
      action = 2; // set next action
      break; // exit action
    }

    case 2:
    { // drive fast
      Serial.print("action: ");
      Serial.println( action );
      lastAction = action; // remeber where we are
      if ( !driveStraight( FORWARD, setDistance ,FAST, 20 ) ) // drive: Fast , detect obstacles < 20cm )
      { 
        setDistance = setDistance - driveDistanceDone() + 300; // calculate the rest of the distance to go.
        action=3; // react to obstacle in action 10
        break; // exit action
      } // obstacle detected
      setDistance =300; // set distance for next action
      action = 3; // set next action
      break; // exit action
    }
    
    case 3:
    { // drive slow
      Serial.print("action: ");
      Serial.println( action );
      lastAction = action; // remeber where we are
      if ( !driveStraight( FORWARD, setDistance ,SLOW, 5 )) // drive: slow , detect obstacles < 5cm )
      { // obstacle detected
        setDistance = setDistance - driveDistanceDone(); // calculate the rest of the distance to go.
        action=10; // react to obstacle in action 11
        break; // exit action
      }
      action = 4; // set next action
      break; // exit action
    }

    case 4:
    { // turn around
      Serial.print("action: ");
      Serial.println( action );
      lastAction = action; // remember where we are
      driveStop(); // stop
      delay(500); // wait half a second 
      driveTurn(180); // turn around
      driveStop(); // stop 
      loopCount++; // remmber how many times we did this.
      action = 1; // set next action
      break; // exit action
    }

    case 10:
    { // an obstacle is detected: stop and wait for it to go away
      Serial.print("action: ");
      Serial.println( action );
      driveStop(); // stop
      while(distanceSensorCheckObstacle(10)) // check if obstacle has gone
        delay(250); // wait 1/4 second
      action = lastAction; // return to action we were doing.
      break;
    }


    default:
      break; 
  }

  if (loopCount >= MAXLOOP) while(1);
  
}

// end module
