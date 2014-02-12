#include <Arduino.h>
#include <Encoder.h>

Encoder::Encoder(pin_t pinA, pin_t pinB, float distPerCount, int timeout, bool invert) :
  pA(pinA), pB(pinB), fDistPerCount(distPerCount), iTimeout(timeout), isInverted(invert)
{
  pinMode(pA, INPUT);
  pinMode(pB, INPUT);

  fPeriod = {0.0f};
  sLoopCounter = 0;
}
  
long Encoder::getDistance(){
  return fDistPerCount * (float)iCount;
}

float Encoder::getSpeed(){
  if(micros() - lastTime >= timeout){
    fSpeed = 0.0f;
    lastTime = micros() - timeout;
  } else {
    fSpeed = (fDistPerCount / this->getPeriod()) * 1000000.0f;
    if (!bDirection) fSpeed *= -1.0f;
  }
  return fSpeed;
}

long Encoder::getCount(){
  return fCount;
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
  fPeriod = {0.0f};
}

double Encoder::getDistancePerCount(){
  return fDistPerCount;
}

bool Encoder::stopped(){
  return getSpeed() == 0;

}
int Encoder::direction(){
  double s = this->getSpeed();
  if(s > 0){ return  1; }
  if(s < 0){ return -1; }
  return 0;
}

void Encoder::encoderEvent(bool mode){

  // Log current system time.
  long currentTime = micros();

  // Determine instantaneous period
  float instantPeriod = (float)(currentTime - lastTime);
  
  // Snity check (can't be having infinite speeds here!)
  // Note: 1000000 microseconds per second
  // Units: m/count * 1 count / us * 1000000 us/s = m/s
  // if(distancePerCount / instantPeriod * 1000000 > 100){ return; }
  // Commented out for speed purposes
  
  // Overwrite oldest period
  fPeriod[sLoopCounter] = instantPeriod;
  (++sLoopCounter) %= AVERAGE_COUNT;

  // Update lastTime with currentTime
  lastTime = currentTime;
  
  // Increment or decrement the count
  if(digitalRead(pinA) == (digitalRead(pinB) ^ !invert ^ mode)) {
    count++;
    bDirection = true;
  } else {
    count--;
    bDirection = false;;
  }
}