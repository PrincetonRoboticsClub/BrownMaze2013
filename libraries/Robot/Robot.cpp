#include <Robot.h>
#include <Motor.h>
#include <Encoder.h>
#include <PID.h>
#include <Arduino.h>

// Fudge Factors
#define ANGLE_DEADBAND 0.05f
#//define ANGLE_SPD_DEADBAND 5.0f
#define POS_DEADBAND 1.8f

/*** Static Functions ***/

float euclidean(float x1, float y1, float x2, float y2) {
	return sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
}

float angleClamp(float angle, float center) {
	while(angle > center + PI) angle -= 2*PI;
	while(angle < center - PI) angle += 2*PI;
	return angle;
}

/*** Class Functions ***/
Robot::Robot(Motor* leftMotor, Motor* rightMotor, Encoder* leftEncoder, Encoder* rightEncoder, float wheelBase, float maxSpeed)
{
	mLeft = leftMotor;
	mRight = rightMotor;
	eLeft = leftEncoder;
	eRight = rightEncoder;

	// very important!
	eLeft->reset();
	eRight->reset();

	fWheelBase = wheelBase;
	fMaxSpeed = maxSpeed;

	fX = fY = fAngle = 0.0f;
	sCurrentState = kManual;
	targetX = targetY = targetAngle = 0.0f;

	lLastCountLeft = lLastCountRight = 0L;
}

Robot::~Robot() {
	delete mLeft;
	delete mRight;
	delete eLeft;
	delete eRight;
}

void Robot::rawTank(float leftSpeed, float rightSpeed) {
	mLeft->writeSpeed(leftSpeed);
	mRight->writeSpeed(rightSpeed);
	sCurrentState = kManual;
}

void Robot::brake() {
	mLeft->brake();
	mRight->brake();
	sCurrentState = kManual;
}

float Robot::getX() {
	return fX;
}
float Robot::getY() {
	return fY;
}
float Robot::getAngle() {
	return fAngle;
}
float Robot::getSpeed() {
  return (eLeft->getSpeed() + eRight->getSpeed()) * 0.5;
}
float Robot::getAngularSpeed() {
  return (eRight->getSpeed() - eLeft->getSpeed()) / fWheelBase;
}
float Robot::getLeftSpeed() {
	return eLeft->getSpeed();
}
float Robot::getRightSpeed() {
	return eRight->getSpeed();
}

State Robot::getState() {
	return sCurrentState;
}
Motor* Robot::getLeftMotor() {
	return mLeft;
}
Motor* Robot::getRightMotor() {
	return mRight;
}
Encoder* Robot::getLeftEncoder() {
	return eLeft;
}
Encoder* Robot::getRightEncoder() {
	return eRight;
}

void Robot::tank(float leftSpeed, float rightSpeed) {
	if(sCurrentState != kManual) {
		mLeft->writeSpeed(pLeftSpeed->compute(eLeft->getSpeed(), leftSpeed));
		mRight->writeSpeed(pRightSpeed->compute(eRight->getSpeed(), rightSpeed));
	} // End if
} // End tank(float, float)

void Robot::arcade(float straight, float turn) {
	float aStraight = abs(straight);
	float aTurn = abs(turn);

	float left = max(aStraight, aTurn);
	float right = aStraight - aTurn;

	if (aTurn != turn) {
		float temp = right;
		right = left;
		left = temp;
	}

	if(aStraight != straight){
		left  *= -1.0f;
		right *= -1.0f;
	}

	tank(left, right);
}

void Robot::posCompute() {
	long dL = eLeft->getCount()  - lLastCountLeft;
	long dR = eRight->getCount() - lLastCountRight;
	lLastCountLeft += dL;
	lLastCountRight += dR;

	float ds = eLeft->getDistancePerCount() * (float)(dR+dL) * 0.5f;
	fX     	+= ds*cos(fAngle);
	fY		+= ds*sin(fAngle);
	fAngle	+= eLeft->getDistancePerCount()/fWheelBase * (float)(dL-dR);
}

void Robot::update() {
	posCompute(); // Update Position Variables
	float straightOut;
	float turnOut;

	// State Machine
	switch(sCurrentState) {
		// constants for kMoving
		float straightInput;
		float turnInput;
		float turnSetPoint;

		case kMoving:
			straightInput = euclidean(fX, fY, targetX, targetY);
			straightOut = fMaxSpeed * pPosition->compute(-1.0f * straightInput);
			turnInput = angleClamp(fAngle, targetAngle);
			turnSetPoint = angleClamp(atan2(targetY-fY, targetX-fX), targetAngle);
			//float turnSetPoint = angleClamp(atan2((targetY-fY) * (1.0f+0.1f*cos(targetAngle)), (targetX-fX) * (1.0f+0.1f*sin(targetAngle))), targetAngle);
			turnOut = fMaxSpeed * pDriveAngle->compute(turnInput, turnSetPoint);

			// seems to have almost no effect currently
			//if (straightInput < (1.5f*POS_DEADBAND))
			//	turnOut = fMaxSpeed*pDriveAngle->compute(fAngle, targetAngle);

			if (straightInput < POS_DEADBAND) {
				sCurrentState = kWaiting;
				fAngle = angleClamp(fAngle, 0.0f);
			} else
				arcade(straightOut, turnOut);
			break;
		case kTurning:
			Serial.print(angleClamp(fAngle, targetAngle));
			Serial.print(" -> ");
			Serial.print(targetAngle);
			Serial.print("  :  ");
			Serial.print(ANGLE_DEADBAND);
			Serial.println();
			turnOut = fMaxSpeed*pAngle->compute(angleClamp(fAngle, targetAngle), targetAngle);
			// having angleClamp in the next line is very important!!!
			if(abs(angleClamp(fAngle, targetAngle)-targetAngle) < ANGLE_DEADBAND) {
			//	if(getAngularSpeed() < ANGLE_SPD_DEADBAND) {
					sCurrentState = kWaiting;
					fAngle = angleClamp(fAngle, 0.0f);
			//	} else {
			//		pLeftSpeed->reset();
			//		pRightSpeed->reset();
			//	}
			}
			else {
				arcade(0.0f, turnOut);
			}
			break;
		case kWaiting:
			// makes behavior smoother and gets rid of whining noise
			mLeft->brake();
			mRight->brake();

			// prevents build up
			pLeftSpeed->reset();
			pRightSpeed->reset();

			break;
	}
}

void Robot::setSetPosition(float newX, float newY) {
	if(sCurrentState != kManual) {
		targetX = newX;
		targetY = newY;
		targetAngle = fAngle;
		sCurrentState = kMoving;
	}
}

void Robot::setSetAngle(float newAngle) {
	if(sCurrentState != kManual) {
		targetAngle = newAngle;
		fAngle = angleClamp(fAngle, PI);
		sCurrentState = kTurning;
	}
}

void Robot::manual() {
	sCurrentState = kManual;
	targetX = targetY = targetAngle = 0.0f;
}

void Robot::begin(PID* pLS, PID* pRS, PID* pP, PID* pDA, PID* pA) {
	pLeftSpeed = pLS;
	pRightSpeed = pRS;
	pPosition = pP;
	pDriveAngle = pDA;
	pAngle = pA;

	sCurrentState = kWaiting;
}

void Robot::wait(long ms) {
	sCurrentState = kWaiting;

	long startTime = millis();
	while (millis() - startTime < ms) {
		update();
	}
}
void Robot::waitForNext() {
	waitForNext(0);
}
void Robot::waitForNext(long ms) {
	while (sCurrentState != kWaiting) {
		update();
	}
	wait(ms);
}

void Robot::reset() {
	sCurrentState = kWaiting;
	update();
	eLeft->reset();
	eRight->reset();
}

void Robot::changeSetX(float dx) {
	setSetPosition(targetX + dx, targetY);
}
void Robot::changeSetY(float dy) {
	setSetPosition(targetX, targetY + dy);
}
void Robot::changeSetAngle(float da) {
	setSetAngle(targetAngle + da);
}