#include <Arduino.h>
#include <Encoder.h>

void Encoder::init(pin_t pinA, pin_t pinB, float distPerCount, int timeout, bool invert) {
  pA = pinA;
  pB = pinB;
  fDistPerCount = distPerCount;
  iTimeout = timeout;
  isInverted = invert;

  // Set both pins as digital inputs.
  pinMode(pA, INPUT);
  pinMode(pB, INPUT);

  // Reset the period averager.
  for (int i = 0; i < AVERAGE_COUNT; i++)
    fPeriod[i] = 0.0f;
  sLoopCounter = 0;

  // Reset Counter
  iCount = 0L;
}
  
float Encoder::getDistance(){
  // Units: length/count * count = length
  return fDistPerCount * (float)iCount;
}

float Encoder::getSpeed(){
  if(micros() - iLastTime >= iTimeout){
    fSpeed = 0.0f;
    iLastTime = micros() - iTimeout;
  } else {
    // Units: (length/count) / (micros/count) * (micros/second) = length/second
    fSpeed = (fDistPerCount / this->getPeriod()) * 1000000.0f;
    if (!bDirection) fSpeed *= -1.0f;
  }
  return fSpeed;
}

long Encoder::getCount(){
  return iCount;
}

float Encoder::getPeriod() {
  int i;
  float average = 0.0f;
  for(i = 0; i < AVERAGE_COUNT; i++) {
    average += fPeriod[i];
  }
  average /= (float)AVERAGE_COUNT;
  return average;
}

void Encoder::setTimeout(long timeout){
  iTimeout = timeout;
}

void Encoder::reset(){
  iCount = 0;
  fSpeed = 0;

  for (int i = 0; i < AVERAGE_COUNT; i++)
    fPeriod[i] = 0.0f;
}

float Encoder::getDistancePerCount(){
  return fDistPerCount;
}

bool Encoder::stopped(){
  return getSpeed() == 0;

}
int Encoder::direction(){
  float s = this->getSpeed();
  if(s > 0){ return  1; }
  if(s < 0){ return -1; }
  return 0;
}

void Encoder::encoderEvent(bool mode){

  // Log current system time.
  long currentTime = micros();

  // Determine instantaneous period
  float instantPeriod = (float)(currentTime - iLastTime);
  
  // Overwrite oldest period
  fPeriod[sLoopCounter] = instantPeriod;
  (++sLoopCounter) %= AVERAGE_COUNT;

  // Update lastTime with currentTime
  iLastTime = currentTime;
  
  // Increment or decrement the count
  if(digitalRead(pA) == (digitalRead(pB) ^ !isInverted ^ mode)) {
    iCount++;
    bDirection = true;
  } else {
    iCount--;
    bDirection = false;
  }
}