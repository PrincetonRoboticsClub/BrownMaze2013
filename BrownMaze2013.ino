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
#define WHEEL_BASE 10.2f
#define MAX_SPEED 90.0f

#define COUNT_PER_REV 1200
#define TIMEOUT 10000

/** PID Constants **/
#define TANK_I_GAIN 0.03
#define POS_P_GAIN 0.01
#define ANGLE_P_GAIN 0.08

Robot* r;
int programCount;


// Program Sequence
void programSequence(int progCount) {
  switch(progCount) {
    case 0:
    r->setSetPosition(60.0f, 0.0f);
    Serial.println("STRAIGHT X");
    break;
    case 1:
    r->setSetAngle(PI * 0.5f);
    Serial.println("TURN PI/2");
    break;
    case 2:
    r->setSetPosition(60.0f, 60.0f);
    Serial.println("STRAIGHT Y");
    break;
    case 3:
    r->setSetAngle(PI);
    Serial.println("TURN PI");
    break;
    case 4:
    r->setSetPosition(0.0f, 60.0f);
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

void setup() {
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
  r->begin(TANK_I_GAIN, ANGLE_P_GAIN, POS_P_GAIN);
  
  /* Only Uncomment One of the Below at a Time */
  
  // Move to Point
  //r->setSetPosition(120.0f, 0.0f);
  
  // Turn to Angle
  //r->setSetAngle(PI * 0.5f);
  
  // Use Program Sequence Counter
  programCount = 0;
  programSequence(programCount);
  
  // Start In Manual Mode (Not SetPoints Allowed)
  //r->manual();
}

void loop() {
  
  if(r->getState() == kWaiting)
    programSequence(++programCount);
  /*
  Serial.print("Left: ");
  Serial.print(r->getLeftSpeed());
  Serial.print("; Right: ");
  Serial.println(r->getRightSpeed());
  */
  Serial.print("Angle: ");
  Serial.print(r->getAngle());
  Serial.print("; X: ");
  Serial.print(r->getX());
  Serial.print("; Y: ");
  Serial.println(r->getY());
  
  
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
