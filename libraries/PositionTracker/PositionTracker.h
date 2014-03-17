/***************************************************
 * 
 * Probotics - Princeton University
 * 
 * PositionTracker: uses 2 encoders to keep
 *   track of position, angle, velocity, and
 *   angular velocity
 * 
 * note:
 *   leftEncoderEvent() and rightEncoderEvent()
 *     must be attached to interrupts
 *   compute() should be called frequently
 * 
 ***************************************************/

#ifndef PositionTracker_h
#define PositionTracker_h

//#include <Arduino.h>
#include <Encoder.h>

class PositionTracker{
  public:
    PositionTracker(Encoder* eLeft, Encoder* eRight, float wheelBase);
					
    void compute();
    
    double getXCoord();
    double getYCoord();
    double getAngle();
    
    void reset();

    double getSpeed();
    double getAngularSpeed();
    
    void interruptL1();
    void interruptL2();
    void interruptR1();
    void interruptR2();

  private:
    float fWheelBase;
    Encoder* leftEncoder;
    Encoder* rightEncoder;
    float x, y, angle;
    long leftCount, rightCount;
};

#endif