
#ifndef ROBOT_H
#define ROBOT_H

#include <Motor.h>
#include <Encoder.h>
#include <PID.h>

/**
 * Class Robot
 * Creates a functioning, self-aware robot with two Motors and Encoders.
 **/

 enum State {
 	kManual,
 	kMoving,
 	kTurning,
 	kWaiting
 };

 class Robot {
 private:

 	// Hardware
 	Motor* mLeft;
 	Motor* mRight;
 	Encoder* eLeft;
 	Encoder* eRight;

 	// PID Objects
 	PID* pLeftSpeed;
 	PID* pRightSpeed;
 	PID* pPosition;
 	PID* pDriveAngle;
 	PID* pAngle;

 	// Detect Position and Orientation
 	float fAngle, fX, fY;
 	State sCurrentState;

 	// Hard Dimensions (doubling as fudge-factors)
 	float fWheelBase;
 	float fMaxSpeed;

 	// Set Points
 	float targetX, targetY, targetAngle;
   float lastTX, lastTY, lastTAngle;

 	// Position Tracker
 	long lLastCountLeft, lLastCountRight;
 	void posCompute();

 public:

 	Robot(Motor* leftMotor, Motor* rightMotor, Encoder* leftEncoder, Encoder* rightEncoder, float wheelBase, float maxSpeed);
 	~Robot();

 	/*** Manual Functions ***/
 	// Sets left and right speed manually
 	void rawTank(float leftSpeed, float rightSpeed);
 	// Stops robot manually
 	void brake();

 	/*** Information Functions ***/
 	float getX();
 	float getY();
 	float getAngle();

 	float getSpeed();
 	float getLeftSpeed();
 	float getRightSpeed();
 	float getAngularSpeed();

 	// Getters
 	State getState();
 	Motor* getLeftMotor();
 	Motor* getRightMotor();
 	Encoder* getLeftEncoder();
 	Encoder* getRightEncoder();

 	/*** PID-Looped Functions, Can't Be In Manual Mode ***/
 	// Uses the I-loops to set absolute Motor Speeds
 	void tank(float leftSpeed, float rightSpeed);
 	void arcade(float straight, float turn);

 	/** State Functions **/
 	void update(); // Called Every Loop, Updates PID Loops
 	void setSetPosition(float newX, float newY); // Robot's New Target Point, changes to Moving Mode
 	void setSetAngle(float newAngle); // Robot's New Target Angle, changes to Turning Mode

 	void manual(); // Changes to Manual Mode
 	void begin(PID* pLS, PID* pRS, PID* pP, PID* pDA, PID* pA); // Changes to Non-Manual Mode

   void wait(long ms); // wait for ms milliseconds
   void waitForNext(); // wait for the robot to be in state kWaiting
   void waitForNext(long ms); // wait for state to be kWaiting, then wait ms millis

   void reset(); // reset the robot

   void changeSetX(float dx); // change target x by dx
   void changeSetY(float dy); // change target y by dy
   void changeSetAngle(float da); // change target angle by da

   int getDirection();

   float getAngleToTarget(float distance);

	void moveDirection(int direction, uint8_t n);
 };
#endif
