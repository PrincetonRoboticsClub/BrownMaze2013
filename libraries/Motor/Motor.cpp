#include <Arduino.h>
#include <Motor.h>


Motor::Motor(pin_t a1, pin_t a2, pin_t speedPinEN, bool invert) :
	pA1(a1), pA2(a2), pEN(speedPinEN), bInverted(invert), fCurrentSpeed(0.0)
{
	pinMode(pA1,	OUTPUT);
	pinMode(pA2,	OUTPUT);

	pinMode(pEN,	OUTPUT);
}

void Motor::stop() {
	analogWrite(pEN,	MOTOR_MAX);

	digitalWrite(pA1,	LOW);
	digitalWrite(pA2,	LOW);

	fCurrentSpeed = 0.0;
}

void Motor::coast() {
	analogWrite(pEn, 0);

	fCurrentSpeed = 0.0;
}

void Motor::writeSpeed(float speed) {
	fCurrentSpeed = min(abs(speed*MOTOR_MAX), MOTOR_MAX);

	analogWrite(pEN,	fCurrentSpeed);

	digitalWrite(pA1,	(speed > 0) ^ bInverted);
	digitalWrite(pA1,	(speed < 0) ^ bInverted);
}

void Motor::getSpeed() {
	return fCurrentSpeed;
}