#include <PID.h>
#include <Encoder.h>
#include <Motor.h>
#include <Robot.h>
#include <Arduino.h>
#include <PinChangeInt.h>
#include <WallSensor.h>

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

// IR Sensors
#define PIN_IRL A1
#define PIN_IRF A2
#define PIN_IRR A3

/** Robot Dimenion Definitions (cm or cm/s) **/
#define WHEEL_RADIUS 1.708278f
#define WHEEL_BASE 8.84//8.5f 8.84
#define MAX_SPEED 60.0f

#define COUNT_PER_REV 1200
#define TIMEOUT 10000

/** PID Constants **/

#define TANK_I_GAIN 0.14
#define POS_P_GAIN 0.03 // 0.023
#define ANGLE_P_GAIN 0.25 // 0.25
#define ANGLE_I_GAIN 0.00
#define ANGLE_D_GAIN 0.00
#define DANGLE_P_GAIN 0.0 // 0.15
#define DANGLE_D_GAIN 0.0 // 1.7

// length of a maze tile (18cm) 22.0
#define TILE_LENGTH 18.5f
// ms delay for waits
#define INSTRUCTION_DELAY 400

Robot* r;
WallSensor* ws;

void setup() {
  delay(1000);

  // stop switch pin
  pinMode(13, INPUT_PULLUP);

  ws = new WallSensor(PIN_IRF, PIN_IRR, PIN_IRL);

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
  new PID(POS_P_GAIN, 0.0, 0.0, 0.3), // Speed P Controller
  new PID(DANGLE_P_GAIN, 0.0, DANGLE_D_GAIN), // Drive Angle PD Controller
  new PID(ANGLE_P_GAIN, ANGLE_I_GAIN, ANGLE_D_GAIN) // Angle PID Controller
  );
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
  r->changeSetX(2*TILE_LENGTH);
  r->waitForNext(INSTRUCTION_DELAY);
  r->changeSetAngle(PI);
  r->waitForNext(INSTRUCTION_DELAY);
  r->changeSetX(-2*TILE_LENGTH);
  r->waitForNext(INSTRUCTION_DELAY);
  r->changeSetAngle(PI);
  r->waitForNext(INSTRUCTION_DELAY);
  */
  

  /*
  //--------------------------------------------
  // drive in a maze and avoid walls
  //--------------------------------------------
  int direction = r->getDirection();
  int* walls = ws->getWalls(direction);

  r->changeSetAngle(0.0);

  if (walls[direction]) {
    if (!walls[(direction + 1) % 4]) {
      turnRight();
    }
    else if (!walls[(direction + 3) % 4]) {
      turnLeft();
    }
    else {
      turnAround();
    }
  }
  moveForward(TILE_LENGTH);
  //--------------------------------------------
  */
  
  // drive a small portion of the maze
  //doLoop();

  // back and forth 5 tiles
  //doStraight(5);
  //doStraight(2);

  // move in a square
  //doSquare(TILE_LENGTH);

  // repeadedly move forward
  //moveForward(TILE_LENGTH);

  // repeadetly turn
  turnRight();
  //turnLeft();

  // back and forth
  //moveForward(TILE_LENGTH);
  //turnAround();

  //r->update();
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
  Serial.print("dir: ");
  Serial.print(r->getDirection());
  Serial.print("; Angle: ");
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

//--------------------------------------------

// Basic directional / relative movement

void moveForward(float distance) {
  //fixAngle();
  int direction = r->getDirection();

  //r->setSetAngle(r->getAngleToTarget(distance));
  //r->waitForNext(INSTRUCTION_DELAY);

  if (direction == 0) {
    r->changeSetX(distance);
  }
  else if (direction == 1) {
    r->changeSetY(distance);
  }
  else if (direction == 2) {
    r->changeSetX(-distance);
  }
  else if (direction == 3) {
    r->changeSetY(-distance);
  }
  else {
    r->wait(100000);
  }
  r->waitForNext(INSTRUCTION_DELAY);
}

void turnRight() {
  r->changeSetAngle(PI/2);
  r->waitForNext(INSTRUCTION_DELAY);
}

void turnLeft() {
  r->changeSetAngle(-PI/2);
  r->waitForNext(INSTRUCTION_DELAY);
}

void turnAround() {
  turnRight();
  turnRight();
}

void fixAngle() {
  r->changeSetAngle(0);
  r->waitForNext(INSTRUCTION_DELAY);
}

//--------------------------------------------

// Movement Patterns

void doSquare(float size) {
  for (int i = 0; i < 4; i++) {
    moveForward(TILE_LENGTH);
    turnRight();
    //turnLeft();
  }
}

void doLoop() {
  // forward 3
  moveForward(TILE_LENGTH);
  moveForward(TILE_LENGTH);
  moveForward(TILE_LENGTH);
  // right 1
  turnRight();
  // forward 1
  moveForward(TILE_LENGTH);
  // right 1
  turnRight();
  // forward 2
  moveForward(TILE_LENGTH);
  moveForward(TILE_LENGTH);
  // left 1
  turnLeft();
  // forward 3
  moveForward(TILE_LENGTH);
  moveForward(TILE_LENGTH);
  moveForward(TILE_LENGTH);
  // left 1
  turnLeft();
  // forward 3
  moveForward(TILE_LENGTH);
  moveForward(TILE_LENGTH);
  moveForward(TILE_LENGTH);
  // left 1
  turnLeft();
  // forward 2
  moveForward(TILE_LENGTH);
  moveForward(TILE_LENGTH);
  // left 1
  // forward 1 - issue?!
  //r->changeSetX(-TILE_LENGTH);
  //r->waitForNext(INSTRUCTION_DELAY);
  moveForward(TILE_LENGTH);
  // right 2
  turnAround();
  // forward 1
  moveForward(TILE_LENGTH);
  // left 1
  turnLeft();
  // forward 1
  moveForward(TILE_LENGTH);
  // right 1
  turnRight();
  // forward 1
  moveForward(TILE_LENGTH);
  // left 1
  turnLeft();
  // forward 1
  moveForward(TILE_LENGTH);
  // left 1
  turnLeft();
  // forward 5
  moveForward(TILE_LENGTH);
  moveForward(TILE_LENGTH);
  moveForward(TILE_LENGTH);
  moveForward(TILE_LENGTH);
  moveForward(TILE_LENGTH);
  // right 2
  turnAround();
}

void doStraight(int count) {
  for (int i = 0; i < count; i++) {
    //fixAngle();
    moveForward(TILE_LENGTH);
  }
  //fixAngle();
  turnRight();
  //fixAngle();
  turnRight();
}