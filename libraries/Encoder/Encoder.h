/***********************************************
 * 
 * Probotics - Princeton University
 * 
 * Encoder: uses 4x decoding to decode 2 signals
 *   keeps track of speed and distance
 * 
 ***********************************************/


#ifndef Encoder_h
#define Encoder_h

#include <Arduino.h>

typedef int pin_t;
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


  public:
    Encoder(pin_t pinA, pin_t pinB, float radius, float countPerRotation, int timeout, bool invert) :
        Encoder(pinA, pinB, 2*PI*radius/countPerRotation, timeout, invert) {}

    Encoder(pin_t pinA, pin_t pinB, float radius, float countPerRotation, bool invert) :
        Encoder(pinA, pinB, radius, countPerRotation, DEFAULT_TIMEOUT, invert) {}

	Encoder(pin_t pinA, pin_t pinB, float distPerCount, bool invert) :
        Encoder(pinA, pinB, distPerCount, DEFAULT_TIMEOUT, invert) {}

    Encoder(pin_t pinA, pin_t pinB, float distPerCount, int timeout, bool invert);
  
    float getDistance();
    float getSpeed();
    float getCount();
    float getPeriod();
    void setTimeout(long timeout);
    void reset();
    float getDistancePerCount();
    bool stopped();
    int direction();

    void encoderEvent(bool mode);
};

#endif