#include <PID.h>
#include <Encoder.h>
#include <Motor.h>
#include <Robot.h>
#include <Arduino.h>
#include <PinChangeInt.h>

/** Robot Pin Definitions **/

// Left Motor
#define PIN_LA 8
#define PIN_LB 7
#define PIN_LN 6

// Right Motor
#define PIN_RA 3
#define PIN_RB 4
#define PIN_RN 5

// Left Encoder
#define PIN_ELA 11
#define PIN_ELB 12

// Right Encoder
#define PIN_ERA 9
#define PIN_ERB 10

/** Robot Dimenion Definitions (cm or cm/s) **/
#define WHEEL_RADIUS 2.0f
#define WHEEL_BASE 10.0f
#define MAX_SPEED 70.0f // 70.0f

#define COUNT_PER_REV 1200
#define TIMEOUT 10000

/** PID Constants **/
#define TANK_I_GAIN 0.03 // 0.03, 0.09
#define POS_P_GAIN 0.025 // 0.01, 0.05
#define ANGLE_P_GAIN 0.13 // 0.095
#define ANGLE_I_GAIN 0.00 // 0.01
#define ANGLE_D_GAIN 0.00 // 0.02
#define DANGLE_P_GAIN 0.9 // 0.4, 0.7
#define DANGLE_D_GAIN 0.7 // 0.1, 0.3

#define TILE_LENGTH 22.0f // 18cm

// could change angle behavior to have a different max speed for
// turning than straight -- this would help prevent overshoot

Robot* r;
int programCount = 0;


// Program Sequence
void programSequence(int progCount) {
  switch(progCount) {
  case 0:
    r->setSetPosition(TILE_LENGTH, 0.0f);
    Serial.println("STRAIGHT X");
    break;
  case 1:
    r->setSetAngle(PI * 0.5f);
    Serial.println("TURN PI/2");
    break;
  case 2:
    r->setSetPosition(TILE_LENGTH, TILE_LENGTH);
    Serial.println("STRAIGHT Y");
    break;
  case 3:
    r->setSetAngle(PI);
    Serial.println("TURN PI");
    break;
  case 4:
    r->setSetPosition(0.0f, TILE_LENGTH);
    Serial.println("STRAIGHT -X");
    break;
  case 5:
    r->setSetAngle(PI * 1.5f);
    Serial.println("TURN 3PI/2");
    break;
  case 6:
    r->setSetPosition(0.0f, 0.0f);
    break;
  case 7:
    r->setSetAngle(0.0f);
    break;
  default:
    // Do Nothing!
    Serial.println("DONE!");
  }
}

//float fTileLength = 2100.0*2*PI*WHEEL_RADIUS/COUNT_PER_REV; // 21.99

void setup() {
  // stop switch pin
  pinMode(13, INPUT_PULLUP);

  r = new Robot(
  new Motor(PIN_LA, PIN_LB, PIN_LN, false), 
  new Motor(PIN_RA, PIN_RB, PIN_RN, false),
  new Encoder(PIN_ELA, PIN_ELB, WHEEL_RADIUS, COUNT_PER_REV, TIMEOUT, true),
  new Encoder(PIN_ERA, PIN_ERB, WHEEL_RADIUS, COUNT_PER_REV, TIMEOUT, false),
  WHEEL_BASE,
  MAX_SPEED
    );

  PCattachInterrupt(PIN_ELA, isrLA, CHANGE);
  PCattachInterrupt(PIN_ELB, isrLB, CHANGE);

  PCattachInterrupt(PIN_ERA, isrRA, CHANGE);
  PCattachInterrupt(PIN_ERB, isrRB, CHANGE);

  Serial.begin(9600);
  r->begin(
  new PID(0.0, TANK_I_GAIN, 0.0), // Left I Controller
  new PID(0.0, TANK_I_GAIN, 0.0), // Right I Controller
  new PID(POS_P_GAIN, 0.0, 0.0), // Speed P Controller
  new PID(DANGLE_P_GAIN, 0.0, DANGLE_D_GAIN), // Drive Angle PD Controller
  new PID(ANGLE_P_GAIN, ANGLE_I_GAIN, ANGLE_D_GAIN) // Angle PID Controller
  );

  /* Only Uncomment One of the Below at a Time */

  // Move to Point
  //r->setSetPosition(50.0f, 0.0f);
  //r->setSetPosition(TILE_LENGTH, 10.0f);

  // Turn to Angle
  //r->setSetAngle(PI/2.0);

  // Use Program Sequence Counter
  //programCount = 0;
  //programSequence(programCount);

  // Start In Manual Mode (Not SetPoints Allowed)
  //r->manual();
}

int n = 1;

void loop() {

  //
  while (digitalRead(13)) {
    r->brake();
    delay(500);
    //r->reset();
  }
  
  if(r->getState() == kWaiting) {
    programSequence(programCount++);
    long t = millis();
    while (millis() - t < 2000) {r->update();}
  }

  /*
  Serial.print("Left: ");
   Serial.print(r->getLeftSpeed());
   Serial.print("; Right: ");
   Serial.println(r->getRightSpeed());
   */

   /*
  Serial.print("Angle: ");
  Serial.print(r->getAngle());
  Serial.print("; X: ");
  Serial.print(r->getX());
  Serial.print("; Y: ");
  Serial.println(r->getY());
  */

  // run in a straight line forever! - square by square
  /*while (r->getState() != kWaiting) {r->update();}
  long t = millis();
  while (millis() - t < 200) {r->update();}
  r->setSetPosition(n++ * TILE_LENGTH, 0.0f);
  */

  r->update();
}


/** Encoder Interrupts **/

void isrLA() {
  r->getLeftEncoder()->encoderEvent(true);
}
void isrLB() {
  r->getLeftEncoder()->encoderEvent(false);
}

void isrRA() {
  r->getRightEncoder()->encoderEvent(true);
}
void isrRB() {
  r->getRightEncoder()->encoderEvent(false);
}

