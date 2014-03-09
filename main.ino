#include "defs.h"

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

	ms.brake();
}

void loop() {
	double straight, turn;
	straight = distancePID->compute(pt->getXCoord(), 20);
	turn = anglePID->compute(pt->getAngle(), 0);

	if(abs(20 - pt->getXCoord()) < 0.1)
		ms.brake();
	else ms.arcade(straight, turn);
}