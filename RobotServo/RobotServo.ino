/*
RobotServo

V0.2  05-11-2016 JD
      Set to default values

V0.1  05-11-2016 JD
      Original version

TowerPro Micro SG90 Wiring Colours:

    Yellow = Control Signal
    Red    = +5V
    Brown  = Gnd.
*/

#include <Servo.h> 
#define SERVOFRONT  90 // Front position Servo 90 + front deviation (+10). Normal 90 
#define SERVORIGHT   0 // Right position when front deviation > 90 + front deviation (10). Normal 0 (keep view angle symetrical)
#define SERVOLEFT  180 // Left position when front deviation < 90 - front deviation (-). Normal 180 (keep view angle symetrical)

// Servo
  const int servoPin = 9; // pin 9 - Servo 2
  Servo myservo; // create servo object to control a servo 

 
void setup() 
{ 
   myservo.attach(servoPin);  // attaches the servo on pin 9 to the servo object 
} 

void loop() 
{ 
  static int pos;
 
  pos = SERVOFRONT;
  myservo.write(pos); // tell servo to go to position in variable 'pos' 
  delay(5000); // delay between cycles 
   
  for(pos = SERVOFRONT; pos > SERVORIGHT; pos-=5) // goes from front to right / 5 degree step
  {
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(20);                       // waits 20ms for the servo to reach the position 
  } 
  delay(500);                       // wait half a second in rigt position

  for(pos = SERVORIGHT; pos < SERVOLEFT; pos+=5) // goes from right to left / 5 degree step
  {
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(20);                       // waits 20ms for the servo to reach the position 
  } 
  delay(500);                       // waits 15ms for the servo to reach the position 

  for(pos = SERVOLEFT; pos > SERVOFRONT; pos-=5)  // goes from left to front / 5 degree step
  {
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(20);                       // waits 15ms for the servo to reach the position 
    Serial.println(pos);
  } 
}



