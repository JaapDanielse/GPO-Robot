/*

  MotorControl Module

  Motor shield, Adafruit motor shield compatible
  Uses 74HC595 to control 2 x L293D H-Bridge Chipset to drive up to 4 dc motor bi-directional 
  or 2 stepper motors and 2 servo's. For full control use Adafruit library
  
  This module only controls dc motor 1 and 2 
  The shield has a latch register to control the h-bridge drivers
  
*/

//-----------------------------------------------------------------------------------------
// Pin declarations motor shield

#define MOTOR1SPEED 11 // Motor 1 speed pin (D11) - PWM2A
#define MOTOR2SPEED  3 // Motor 2 spped pin (D3) - PWM2B
#define MOTOR_LATCH 12 // Latch Pin (Enable)(D12)
#define MOTOR_SER    8 // Serial data pin (D8) - data: (8bit) Q0=3A Q1=2A Q2=1A Q3=1B Q4=2B Q5=4A Q6=3B Q7=4B
#define MOTOR_EN     7 // Enable pin (D7) - Low to enable
#define MOTOR_CLK    4 // Serial Clock clock pin (D4) - (shift bit)


//-----------------------------------------------------------------------------------------
// Motor control register (bits A and B of one motor control direction )

#define M1A 5 // Motor 1 Left  (Q2) 
#define M1B 4 // Motor 1 Right (Q3)
#define M2A 6 // Motor 2 Left  (Q1)
#define M2B 3 // Motor 2 Right (Q4)


//-----------------------------------------------------------------------------------------
// Macro define
#define pulseHigh(pin) {digitalWrite(pin, HIGH); digitalWrite(pin, LOW);} // pulse pin high macro


//-----------------------------------------------------------------------------------------
void motorControlInit()
{
  pinMode(MOTOR1SPEED, OUTPUT); // set motor speed pin as output
  pinMode(MOTOR2SPEED, OUTPUT); // set motor speed pin as output
  pinMode(MOTOR_LATCH, OUTPUT); // set latch pin as output
  pinMode(MOTOR_SER, OUTPUT); // set serial data pin as output
  pinMode(MOTOR_EN, OUTPUT); // set enable pin as output
  pinMode(MOTOR_CLK, OUTPUT); // set clock pin as output

  digitalWrite(MOTOR1SPEED, LOW); // motor 1 speed low (0)
  digitalWrite(MOTOR2SPEED, LOW); // motor 2 speed low (0)
  digitalWrite(MOTOR_LATCH, LOW); // latch low (0)
  digitalWrite(MOTOR_EN, LOW); // enable low (0)
  digitalWrite(MOTOR_CLK, LOW); // clock low (0)

}


//-----------------------------------------------------------------------------------------
void motorControl(byte motorNumber, byte motorDirection, byte motorSpeed)
{
  static byte motorControlReg = 0; // motor latch register
  byte currentControlReg = 0; // motor latch register
  byte A; // Motor A bit
  byte B; // Motor B bit
  byte SpeedPin; // Motor Speed Pin
   
  if (motorNumber == 1)
  {
    A = M1A; // set A to Motor 1 A bit in control reg
    B = M1B; // set A to Motor 1 B bit in control reg
    SpeedPin = MOTOR1SPEED; // set SpeedPin to motor1 PWM    
  }
  else if (motorNumber == 2)
  {
    A = M2A; // set A to Motor 2 A bit in control reg
    B = M2B; // set B to Motor 2 B bit in control reg
    SpeedPin = MOTOR2SPEED; // set SpeedPin to motor2 PWM     
  }


  if (motorDirection == FORWARD)
  { 
    bitSet(motorControlReg, A); // setup h-brige for forward drive
    bitClear(motorControlReg, B); // 
  } 
  else if (motorDirection == REVERSE)
  { 
    bitClear(motorControlReg, A); // setup h-brige for reverse 
    bitSet(motorControlReg, B); // 
  }
  else // (motorDirection == STOP)
  {
    bitClear(motorControlReg, A); // setup h-brige for stop (short circuit) 
    bitClear(motorControlReg, B); // 
  }

  currentControlReg = motorControlReg; // copy control reg var to preserve 
  for (int i=0; i<8; i++, currentControlReg >>= 1) // shift right one bit
  { 
    digitalWrite(MOTOR_SER, currentControlReg & 0x01 ); // write bit to motor control latch
    pulseHigh(MOTOR_CLK); // clock in bit
  }
  pulseHigh(MOTOR_LATCH); // latch register complete: clock in latch register 

  analogWrite(SpeedPin,  motorSpeed); // set PWM to required motor speed

}


//-----------------------------------------------------------------------------------------
void motorSpeed (byte motorNumber, byte motorSpeed)
{
  if (motorNumber == 1)
    analogWrite(MOTOR1SPEED,  motorSpeed); // set PWM to required motor speed
  if (motorNumber == 2)
    analogWrite(MOTOR2SPEED,  motorSpeed); // set PWM to required motor speed
}


// end module

