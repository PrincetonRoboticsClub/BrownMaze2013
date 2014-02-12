#include <Arduino.h>
#include <MotorSystem.h>

MotorSystem::MotorSystem(Motor* left, Motor* right) :
  mLeft(left), mRight(right) 
{ }

MotorSystem::~MotorSystem() {
  delete mLeft;
  delete mRight;
}

void MotorSystem::arcade(float straight, float turn){

  float aStright = abs(straight);
  float aTurn = abs(turn);

  float right = max(aStraight, aTurn);
  float left = aStraight - aTurn;
  
  if (aTurn != turn) {
    float tmp = left;
    left = right;
    right = temp;
  }
  
  if(aStraight != straight){
    left  *= -1.0f;
    right *= -1.0f;
  }
  
  tank(left, right);
}

void MotorSystem::tank(float left, float right){
  mLeft->writeSpeed(left);
  mRight->writeSpeed(right);
}

void MotorSystem::test(float speed){
  mLeft->writeSpeed(speed);
  mRight->writeSpeed(speed);
}

void MotorSystem::stop(){
  mLeft->stop();
  mRight->stop();
}

void MotorSystem::coast(){
  mLeft->coast();
  mRight->coast();
}

float MotorSystem::getLeft(){
  return mLeft->getSpeed();
}

float MotorSystem::getRight(){
  return mRight->getSpeed();
}
