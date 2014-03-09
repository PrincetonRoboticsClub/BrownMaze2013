#include <Arduino.h>
#include <PositionTracker.h>
#include <Encoder.h>

PositionTracker::PositionTracker(Encoder* eLeft, Encoder* eRight, float wheelBase)
        : fWheelBase(wheelBase), leftEncoder(eLeft), rightEncoder(eRight) {
  reset();
}

void PositionTracker::compute(){
  long dL = leftEncoder->getCount()  - leftCount;
  long dR = rightEncoder->getCount() - rightCount;
  leftCount += dL;
  rightCount += dR;
  
  double ds = leftEncoder->getDistancePerCount() * (dR+dL)/2;
  x     += ds*cos(angle);
  y     += ds*sin(angle);
  angle += leftEncoder->getDistancePerCount()/fWheelBase * (dL-dR);
}

double PositionTracker::getXCoord(){ return x; }
double PositionTracker::getYCoord(){ return y; }
double PositionTracker::getAngle(){ return angle; }

void PositionTracker::reset(){
  x          = 0;
  y          = 0;
  angle      = 0;
  leftCount  = 0;
  rightCount = 0;

  leftEncoder->reset();
  rightEncoder->reset();
}

double PositionTracker::getSpeed(){
  return (leftEncoder->getSpeed() + rightEncoder->getSpeed()) / 2;
}
double PositionTracker::getAngularSpeed(){
  return (rightEncoder->getSpeed() - leftEncoder->getSpeed()) / fWheelBase;
}