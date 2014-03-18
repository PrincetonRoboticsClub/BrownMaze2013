// Motor Pin Numbers
#define PIN_1A 8
#define PIN_1B 7
#define PIN_1N 6

#define PIN_2A 3
#define PIN_2B 4
#define PIN_2N 5

// Encoder Pin Numbers
#define PIN_E1A 11
#define PIN_E1B 12

#define PIN_E2A 9
#define PIN_E2B 10

// Dimensions (cm)
#define WHEEL_RADIUS 2
#define WHEEL_BASE 8.5

#define COUNT_PER_REV 1200

// Other Constants
#define TIMEOUT 10000

#include <Encoder.h>
#include <Motor.h>
#include <MotorSystem.h>
#include <PositionTracker.h>
#include <PID.h>
#include <PinChangeInt.h>

MotorSystem* ms;

// Encoders and Motors
Encoder enc1 (PIN_E1A, PIN_E1B, WHEEL_RADIUS, COUNT_PER_REV, TIMEOUT, true);
Encoder enc2 (PIN_E2A, PIN_E2B, WHEEL_RADIUS, COUNT_PER_REV, TIMEOUT, false);
                     
PID leftPID(0, 0.05, 0);
PID rightPID(0, 0.05, 0);


void setup() {  
  
  PCattachInterrupt(PIN_E1A, isr1A, CHANGE);
  PCattachInterrupt(PIN_E1B, isr1B, CHANGE);
  
  PCattachInterrupt(PIN_E2A, isr2A, CHANGE);
  PCattachInterrupt(PIN_E2B, isr2B, CHANGE);
  
  
  ms = new MotorSystem(new Motor(PIN_1A, PIN_1B, PIN_1N, false), 
                       new Motor(PIN_2A, PIN_2B, PIN_2N, false));
  
  Serial.begin(9600);
}

double leftSpeed = 0;
double rightSpeed = 0;

int time = millis();
void loop() {
  
  if (millis() - time > 50) {
    time = millis();
    
    leftSpeed = leftPID.compute(enc1.getSpeed(), 30.0);
    rightSpeed = rightPID.compute(enc2.getSpeed(), 30.0);
    
    Serial.print(enc1.getSpeed());
    Serial.print("  ");
    Serial.print(leftSpeed);
    Serial.print("  ");
    Serial.print(enc2.getSpeed());
    Serial.print("  ");
    Serial.print(rightSpeed);
    Serial.print("\n");
    
    ms->tank(leftSpeed, rightSpeed);
    //ms->tank(-12, 15);
    
    
  }
  
  /*
  ms->tank(-2.0, -2.0);
  delay(1000);
  ms->tank(0.5, 0.5);
  delay(1000);
  ms->tank(0.0, 0.0);
  delay(1000);
  ms->tank(-0.5, -0.5);
  delay(1000);
  ms->tank(-1.0, -1.0);
  delay(1000);
  */
}

void isr1A() {
  enc1.encoderEvent(true);
}
void isr1B() {
  enc1.encoderEvent(false);
}

void isr2A() {
  enc2.encoderEvent(true);
}
void isr2B() {
  enc2.encoderEvent(false);
}

