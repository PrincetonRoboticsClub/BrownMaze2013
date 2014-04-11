#include <Robot.h>
#include <Motor.h>
#include <Encoder.h>
#include <PID.h>
#include <Arduino.h>

// Fudge Factors
#define ANGLE_DEADBAND 0.03f
#define POS_DEADBAND 0.75f

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

	float maxAcc = 1.0;
	if(sCurrentState != kManual) {

		//mLeft->writeSpeed(pLeftSpeed->compute(eLeft->getSpeed(), leftSpeed));
		//mRight->writeSpeed(pRightSpeed->compute(eRight->getSpeed(), rightSpeed));
		
		float currentLeftSpeed = eLeft->getSpeed();
		float currentRightSpeed = eRight->getSpeed();
		/*
		if (leftSpeed - currentLeftSpeed >  maxAcc) leftSpeed = currentLeftSpeed + maxAcc;
		else
		if (leftSpeed - currentLeftSpeed < -maxAcc) leftSpeed = currentLeftSpeed - maxAcc;

		if (rightSpeed - currentRightSpeed >  maxAcc) rightSpeed = currentRightSpeed + maxAcc;
		else
		if (rightSpeed - currentRightSpeed < -maxAcc) rightSpeed = currentRightSpeed - maxAcc;
		*/
		float ls = pLeftSpeed->compute(currentLeftSpeed, leftSpeed);
		float rs = pRightSpeed->compute(currentRightSpeed, rightSpeed);

		mLeft->writeSpeed(ls);
		mRight->writeSpeed(rs);
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
	fAngle = angleClamp(fAngle, PI);
}

void Robot::update() {
	posCompute(); // Update Position Variables

	float straightOut = 0.0f;
	float turnOut = 0.0f;


  /*Serial.print("dir: ");
  Serial.print(getDirection());
  Serial.print("; Angle: ");
  Serial.print(getAngle()*180.0/PI);
  Serial.print("; X: ");
  Serial.print(getX());
  Serial.print("; Y: ");
  Serial.println(getY());*/

	float maxAccS = 1.8;
	float maxDecS = 4.0;
	float maxAccT = 5.0;
	float maxDecT = 9.0;
	float currentSpeed = 0.0;
	float currentTurn = 0.0;
	// State Machine
	switch(sCurrentState) {
		// constants for kMoving
		float straightInput;
		float turnInput;
		float turnSetPoint;



		case kMoving:
			//straightInput = euclidean(fX, fY, targetX, targetY);
			straightInput = abs((targetX == lastTX) ? targetY - fY : targetX - fX);
			straightOut = fMaxSpeed * (0.00 + pPosition->compute(-1.0f * straightInput));
			//straightOut = fMaxSpeed * pow(pPosition->compute(-1.0f * straightInput), 2);

			turnInput = angleClamp(fAngle, targetAngle);
			turnSetPoint = angleClamp(atan2(targetY-fY + 3.0*sin(targetAngle), targetX-fX + 3.0*cos(targetAngle)), targetAngle);
			//turnSetPoint = angleClamp(atan2(targetY-fY, targetX-fX), targetAngle);
			turnOut = fMaxSpeed * pDriveAngle->compute(turnInput, turnSetPoint);

			//if (straightInput < (1.5f*POS_DEADBAND))
			//	turnOut = 0.33*fMaxSpeed*(pAngle->compute(fAngle, targetAngle) - 0.50);

			if (straightInput < POS_DEADBAND) {
				sCurrentState = kWaiting;
				fAngle = angleClamp(fAngle, 0.0f);

				pLeftSpeed->reset();
				pRightSpeed->reset();
			} else
				//maxAcc = 1.0;
				currentSpeed = getSpeed();
				if (straightOut - currentSpeed >  maxAccS) straightOut = currentSpeed + maxAccS;
				else
				if (straightOut - currentSpeed < -maxDecS) straightOut = currentSpeed - maxDecS;

				arcade(straightOut, turnOut);
			break;

		case kTurning:

			turnInput = angleClamp(fAngle - targetAngle, 0);

			if(abs(turnInput) < ANGLE_DEADBAND) {
				sCurrentState = kWaiting;
				mLeft->brake();
				mRight->brake();
				fAngle = angleClamp(fAngle, 0.0f);
			}
			else {
				//turnOut = fMaxSpeed*pAngle->compute(angleClamp(fAngle, targetAngle), targetAngle);
				turnOut = fMaxSpeed*pAngle->compute(turnInput);

				if (turnOut - currentTurn >  maxAccT) turnOut = currentTurn + maxAccT;
				else
				if (turnOut - currentTurn < -maxDecT) turnOut = currentTurn - maxDecT;

				arcade(0.0, turnOut);
			}

			break;
		case kWaiting:
			// prevents build up
			pLeftSpeed->reset();
			pRightSpeed->reset();

			// makes behavior smoother and gets rid of whining noise
			mLeft->brake();
			mRight->brake();

			//arcade(0.0, 0.0);

			break;
	}
	/*
	// csv compatible data
	// actual speed
	Serial.print(getSpeed()/70.0);
	Serial.print(", ");
	// target speed
	Serial.print(straightOut/70.0);
	Serial.print(", ");
	// actual angular speed
	Serial.print(getAngularSpeed()/70.0);
	Serial.print(", ");
	// target angular speed
	Serial.print(turnOut/70.0);
	Serial.print(", ");
	// angle error
	Serial.print((angleClamp(targetAngle - getAngle(), 0) * 180.0/PI)/180.0);
	Serial.print("\n");
	*/

}

void Robot::setSetPosition(float newX, float newY) {
	if(sCurrentState != kManual) {
		lastTX = targetX;
		lastTY = targetY;
		targetX = newX;
		targetY = newY;
		sCurrentState = kMoving;

		// prevents build up
		pLeftSpeed->reset();
		pRightSpeed->reset();
	}
}

void Robot::setSetAngle(float newAngle) {
	if(sCurrentState != kManual) {
		lastTAngle = targetAngle;
		targetAngle = angleClamp(newAngle, PI);
		fAngle = angleClamp(fAngle, PI);
		sCurrentState = kTurning;

		// prevents build up
		pLeftSpeed->reset();
		pRightSpeed->reset();
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

int Robot::getDirection() {
	//return (int) (angleClamp(targetAngle, PI) * 2.0/PI);
	int dir = (((int) ((targetAngle + PI/4) * 2.0/PI)) + 4) % 4;
	return dir;
}

float Robot::getAngleToTarget(float distance) {
	float direction = getDirection();

	if (direction == 0) {
    return atan2(targetY - fY, targetX - fX + distance);
  }
  else if (direction == 1) {
    return atan2(targetY - fY + distance, targetX - fX);
  }
  else if (direction == 2) {
    return atan2(targetY - fY, targetX - fX - distance);
  }
  else if (direction == 3) {
    return atan2(targetY - fY - distance, targetX - fX);
  }
  return 0.0; // never get here
}