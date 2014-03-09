#ifndef DEFINITIONS_H
#define DEFINITIONS_H

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

// Included Libraries


#endif



//#include "defs.h"
#include <Encoder.h>
#include <Motor.h>
#include <MotorSystem.h>
#include <PositionTracker.h>
#include <PID.h>

MotorSystem* ms;
PositionTracker* pt;

PID *distancePID;
PID *anglePID;

void setup() {
  // Initialize Motor System
  ms = new MotorSystem(new Motor(PIN_1A, PIN_1B, PIN_1N, true), 
                       new Motor(PIN_2A, PIN_2B, PIN_2N, false));

  // Initialize Position Tracker
  pt = new PositionTracker(new Encoder(PIN_E1A, PIN_E1B, WHEEL_RADIUS, COUNT_PER_REV, TIMEOUT),
         new Encoder(PIN_E2A, PIN_E2B, WHEEL_RADIUS, COUNT_PER_REV, TIMEOUT), WHEEL_BASE);

  distancePID = new PID(0.1, 0, 0);
  anglePID = new PID(1.27, 0, 0);

  ms->brake();
}

void loop() {
  double straight, turn;
  straight = distancePID->compute(pt->getXCoord(), 20);
  turn = anglePID->compute(pt->getAngle(), 0);

  if(abs(20 - pt->getXCoord()) < 0.1)
    ms->brake();
  else 
    ms->arcade(straight, turn);
}
