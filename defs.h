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
#include <Encoder.h>
#include <Motor.h>
#include <MotorSystem.h>
#include <PositionTracker.h>
#include <PID.h>

#endif