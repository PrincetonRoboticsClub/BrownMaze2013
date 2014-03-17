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
#define WHEEL_BASE 7

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
PositionTracker* pt;

PID *distancePID;
PID *anglePID;

void setup() {
  
  // Add Interrupts
  PCattachInterrupt(PIN_E1A, intL1, CHANGE);
  PCattachInterrupt(PIN_E1B, intL2, CHANGE);
  
  PCattachInterrupt(PIN_E2A, intR1, CHANGE);
  PCattachInterrupt(PIN_E2B, intR2, CHANGE);
  
  // Initialize Motor System
  ms = new MotorSystem(new Motor(PIN_1A, PIN_1B, PIN_1N, true), 
                       new Motor(PIN_2A, PIN_2B, PIN_2N, false));

  // Initialize Position Tracker
  pt = new PositionTracker(new Encoder(PIN_E1A, PIN_E1B, WHEEL_RADIUS, COUNT_PER_REV, TIMEOUT, false),
         new Encoder(PIN_E2A, PIN_E2B, WHEEL_RADIUS, COUNT_PER_REV, TIMEOUT, true), WHEEL_BASE);

  distancePID = new PID(0.1, 0, 0);
  anglePID = new PID(0.6, 0.05, 0);

  ms->brake();
  
  Serial.begin(9600);
  
  pinMode(13, INPUT_PULLUP); 
  
}

void loop() {
  double straight, turn;
  
  pt->compute();
  /*
  straight = distancePID->compute(pt->getXCoord(), 20);
  turn = anglePID->compute(pt->getAngle(), 0);
  
  if(digitalRead(13)) {
    if(abs(20 - pt->getXCoord()) < 0.1)
      ms->arcade(0, turn);
    else 
      ms->arcade(straight, turn);
      //ms->tank(0, 0);
  } else {
    ms->brake();
  }
  */
    
  Serial.print("Speed: ");
  Serial.print(pt->getSpeed());
  Serial.print("; Coordinate: (");
  Serial.print(pt->getXCoord());
  Serial.print(", ");
  Serial.print(pt->getYCoord());
  Serial.print("); ");
  Serial.print("Angle: ");
  Serial.println(pt->getAngle() * 180.0 / (3.14159));
}

void intL1() {
  pt->interruptL1();
}

void intL2() {
  pt->interruptL2();
}

void intR1() {
  pt->interruptR1();
}

void intR2() {
  pt->interruptR2();
}
