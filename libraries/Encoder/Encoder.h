/*----------------------------------------------------------------
	Princeton Robotics Club, Princeton University 2013
	
	Encoder Library (Arduino C++)
	
	This is a library for use with quadrature rotary encoders.
	Functions allow the user to get the distance, count, and
	speed of the encoder.
	
	The most important function is encoderEvent, which must be
	attached to a hardware interrupt.  This can only be done in
	the main arduino code.  Here is an example of how:
	
	#include <Encoder.h>
	...
	int pinA = 0;
	int pinB = 1;
	Encoder myEncoder(pinA, pinB, 2.5, 100, false);
	...
	setup(void) {
		attachInterrupt(pinA, encoderEventA, CHANGE);
		attachInterrupt(pinB, encoderEventB, CHANGE);
	}
	...
	// interrupt service routines
	void encoderEventA() {
		myEncoder.encoderEvent(ENCODER_EVENT_A);
	}
	void encoderEventB() {
		myEncoder.encoderEvent(ENCODER_EVENT_B);
	}
	
	Note that 2 interrupts are needed because of the quadrature
	signal.  A change in reading of either pin should cause the
	encoder to update (thus quadrature).

----------------------------------------------------------------*/

#ifndef Encoder_h
#define Encoder_h

#include <Arduino.h>


typedef uint8_t pin_t;
#define AVERAGE_COUNT   6
#define DEFAULT_TIMEOUT 100000

class Encoder{
  private:
    volatile float fSpeed;
    volatile long iCount;
    volatile long iLastTime;
    volatile float fPeriod[AVERAGE_COUNT];
	volatile bool isInverted;

    // Convention: true = forward, false = backward
    volatile bool bDirection;

    float fDistPerCount;
    long iTimeout;
    pin_t pA, pB;

    short sLoopCounter;

    void init(pin_t pinA, pin_t pinB, float distPerCount, int timeout, bool invert);


  public:
    Encoder(pin_t pinA, pin_t pinB, float radius, float countPerRotation, int timeout, bool invert)
        { init(pinA, pinB, 2*PI*radius / countPerRotation, timeout, invert); }

    Encoder(pin_t pinA, pin_t pinB, float radius, float countPerRotation, bool invert)
        { init(pinA, pinB, 2*PI*radius / countPerRotation, DEFAULT_TIMEOUT, invert); }

	Encoder(pin_t pinA, pin_t pinB, float distPerCount, bool invert)
        { init(pinA, pinB, distPerCount, DEFAULT_TIMEOUT, invert); }

    Encoder(pin_t pinA, pin_t pinB, float distPerCount, int timeout, bool invert)
        { init(pinA, pinB, distPerCount, timeout, invert); }
  
    float   getDistance();
    float   getSpeed();
    long    getCount();
    float   getPeriod();
    void    setTimeout(long timeout);
    void    reset();
    float   getDistancePerCount();
    bool    stopped();
    int     direction();

    void encoderEvent(bool mode);
};

#endif