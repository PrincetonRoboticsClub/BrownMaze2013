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
#define MAX_SPEED 70.0f // 70.0 is the real max

#define COUNT_PER_REV 1200
#define TIMEOUT 10000

/** PID Constants **/
#define TANK_I_GAIN 0.03
#define POS_P_GAIN 0.025
#define ANGLE_P_GAIN 0.135
#define ANGLE_I_GAIN 0.00
#define ANGLE_D_GAIN 0.00
#define DANGLE_P_GAIN 0.9
#define DANGLE_D_GAIN 0.7

// length of a maze tile (18cm)
#define TILE_LENGTH 22.0f
// ms delay for waits
#define INSTRUCTION_DELAY 400

Robot* r;

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

  // Start In Manual Mode (Not SetPoints Allowed)
  //r->manual();
}

void loop() {

  // stop switch
  // maybe this can be built in to update()??
  while (digitalRead(13)) {
    r->reset();
    delay(1000);
    r->reset();
  }

  /*
  // drive a small portion of the maze
  r->wait(INSTRUCTION_DELAY);
  // forward 3 tiles
  r->changeSetX(TILE_LENGTH);
  r->waitForNext(INSTRUCTION_DELAY);
  r->changeSetX(TILE_LENGTH);
  r->waitForNext(INSTRUCTION_DELAY);
  r->changeSetX(TILE_LENGTH);
  r->waitForNext(INSTRUCTION_DELAY);
  // quarter turn right
  r->changeSetAngle(PI/2);
  r->waitForNext(INSTRUCTION_DELAY);
  // forward 1 tile
  r->changeSetY(TILE_LENGTH);
  r->waitForNext(INSTRUCTION_DELAY);
  // quarter turn right
  r->changeSetAngle(PI/2);
  r->waitForNext(INSTRUCTION_DELAY);
  // forward 2 tiles
  r->changeSetX(-TILE_LENGTH);
  r->waitForNext(INSTRUCTION_DELAY);
  r->changeSetX(-TILE_LENGTH);
  r->waitForNext();
  // done
  r->wait(1000000);
  */

  //r->waitForNext(INSTRUCTION_DELAY);
  //r->changeSetAngle(PI/2);

  // move in a square
  //r->waitForNext(INSTRUCTION_DELAY);
  //r->changeSetX(TILE_LENGTH);
  //r->waitForNext(INSTRUCTION_DELAY);
  //r->changeSetAngle(PI/2);
  //r->waitForNext(INSTRUCTION_DELAY);
  //r->changeSetY(TILE_LENGTH);
  //r->waitForNext(INSTRUCTION_DELAY);
  //r->changeSetAngle(PI/2);
  //r->waitForNext(INSTRUCTION_DELAY);
  //r->changeSetX(-TILE_LENGTH);
  //r->waitForNext(INSTRUCTION_DELAY);
  //r->changeSetAngle(PI/2);
  //r->waitForNext(INSTRUCTION_DELAY);
  //r->changeSetY(-TILE_LENGTH);
  //r->waitForNext(INSTRUCTION_DELAY);
  //r->changeSetAngle(PI/2);

  // run in a straight line forever! - square by square
  r->waitForNext(INSTRUCTION_DELAY);
  Serial.println("changed x");
  r->changeSetX(TILE_LENGTH);

  //r->update(); // for code in setup
  //printData();
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

void printData() {
  Serial.print("Angle: ");
  Serial.print(r->getAngle()*180.0/PI);
  Serial.print("; X: ");
  Serial.print(r->getX());
  Serial.print("; Y: ");
  Serial.println(r->getY());

  /*
  Serial.print("Left: ");
  Serial.print(r->getLeftSpeed());
  Serial.print("; Right: ");
  Serial.println(r->getRightSpeed());
  */
}