/*
  ChallengeBot 0.1

  Derived from RobotHelloWorld.
  Changes made to:
  DistanceSensor distanceSensorCheckObstacle : 0 indicates obstacle can be ignored.
  Drive > driveTurn : adde obstacle sensing from driveStraight
  added driveTurnDone() to return the number of degrees turned.
  ChallengeBot > loop : Changed behaviour pattern as described in "strategy" comment       

  V0.4   JpD 13-11-2016
         Settings for real arena size 

  V0.3   Jpd 12-11-2016
         Refined, relevant values in defines with explanation. Loopcount on search and destroy.

  V0.2   Jpd 12-11-2016
         Strategy working, move to next quadrant after a target still shaky.

  V0.1   Jpd 11-11-2016
         Original version derived from RobotHelloWorld.

*/

// #define DEBUG //
 
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

  #define MAXLOOP 4 // maximum loops (scan all quadrants and stop)
  #define DETECTIONRANGE 35 // range for object detection (larger means more false echo's
  #define TURNCORRECTION 0 // some value to end up with a near 90 deg angle after the scan.
  #define VIEWANGLETURN 9 // extra turn after detection to center on a object
  
  static int action = 1; // action value
  static int loopCount = 0; // count number of scans
  static int setDistance = 0; // distance variable 
  static int turnToDo = 0; // Portion of the turn still to do

  int i=0;
 
  /* Strategy
   * Strategy is to divide the square where obstacles can be in four quadrants
   * We drive to the center of the first quadrant and turn 135 deg. left. (we now face the wall)
   * 
   * Now we scan for 360 deg. if anything is closer then 50 cm it cant be a wall
   * If this is the case we drive to it, push it over, and reverse back 
   * We remember how far we turned and turn further until 360 deg is complete 
   * then we turn again 90 deg. and proceed to the next quadrant to do the next scan
   */
  
  switch ( action )
  {

// Move to the fist quadrant and face the wall
    case 1:
    { // We drive from the launch pad to the center of the first quadrant not looking at obstacles
      Serial.print("action: "); // show what we are doing
      Serial.println( action );

      speedSensorClear(); // clear counts
      driveStraight( FORWARD, 500 ,FAST, 0 ); // drive: Fast, ignore objects ) move 63 cm to center of first quadrant 
      driveStraight( FORWARD, 130 ,SLOW, 0 ); // drive: Slow, ignore objects )
      driveStop(); // stop
      delay(500); // wait half a second 
      action = 2; // set next action
      break; // exit action
    }

    case 2:
    { // turn facing the wall not looking at obstacles
      Serial.print("action: "); // show what we are doing
      Serial.println( action );
      
      driveTurn( -135, 0); // turn around to face the wall
      driveStop(); // stop 
      delay(500); // wait half a second 
      turnToDo =  360 + 90 + TURNCORRECTION; // set for scan and turn to next quadrant
      action = 3; // set next action
      break; // exit action
    }

    
// Search and destroy
    case 3:
    { // Search
      Serial.print("action: ");  // show what we are doing
      Serial.println( action );

      if (turnToDo < 0) turnToDo = 0; // no negative turn.
      if( !driveTurn( turnToDo, DETECTIONRANGE)) //turn until we see something close enough
      {
        turnToDo -= driveTurnDone(); // remember how far we have turned
        driveTurn( VIEWANGLETURN, 0); // turn a bit further to center on the object
        setDistance = distanceSensorRead(); // check the distance
        turnToDo -= driveTurnDone(); // remember how far we turned now
        
        if ( setDistance <= DETECTIONRANGE && setDistance != 0 ) // if really within range
        {
          driveStop(); // stop 
          delay(500); // wait half a second 
          setDistance = ( setDistance * 10 ) + 30; // distance in mm + 3 cm
          driveStraight(FORWARD, setDistance ,SLOW, 0); // drive to the object and knock it over
          driveStop(); // stop 
          delay(500); // wait half a second 
          driveStraight(REVERSE, setDistance ,SLOW, 0); // drive back again
        }
        action = 3; // we have not yet turned completely
      }
      else // turn completed
      { 
        driveStop(); // stop 
        delay(500); // wait half a second 
        loopCount++; // count the number of quadrants
        action = 4; // set next action
      }
      break; // exit action
    }


// Drive to the next quadrant
    case 4:
    { // drive fast
      Serial.print("action: ");  // show what we are doing
      Serial.println( action );
      
      driveStraight( FORWARD, 800 ,FAST, 0 ); // drive: Fast, ignore objects ) - move 92 cm to next quadrant
      driveStraight( FORWARD, 120 ,SLOW, 0 ); // drive: Slow, ignore objects )
      driveStop(); // stop 
      delay(500); // wait half a second 

      turnToDo = 360 + 90 + TURNCORRECTION; // set for scan and turn to next quadrant
      action = 3; // set next action
      break; // exit action
    }

  }

  if (loopCount >= MAXLOOP)
  {
    driveStop(); // stop 
    delay(500); // wait half a second 
    while(1);
  }
  
}

// end module
