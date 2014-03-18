
#include <Encoder.h>
#include <HBridge.h>
#include <PinChangeInt.h>

// left encoder pins
int pin_E1A = 11;
int pin_E1B = 12;

// right encoder pins
int pin_E2A = 9;
int pin_E2B = 10;

// left motor pins
int pin_M1A = 8;
int pin_M1B = 7;
int pin_M1N = 6;

// right motor pins
pin_t pin_M2A = 3;
pin_t pin_M2B = 4;
pin_t pin_M2N = 5;


int cprev = 1200;

// Encoders and Motors
Encoder enc1 (pin_E1A, pin_E1B, 2, 1200, 10000, false);
Encoder enc2 (pin_E2A, pin_E2B, 2, 1200, 10000, true);

HBridge m1 (pin_M1A, pin_M1B, pin_M1N, true);
HBridge m2 (pin_M2A, pin_M2B, pin_M2N, false);

void setup() {
  pinMode(pin_M1A, OUTPUT);
  pinMode(pin_M1B, OUTPUT);
  
  pinMode(pin_M2A, OUTPUT);
  pinMode(pin_M2B, OUTPUT);
  
  PCattachInterrupt(pin_E1A, isr1A, CHANGE);
  PCattachInterrupt(pin_E1B, isr1B, CHANGE);
  
  PCattachInterrupt(pin_E2A, isr2A, CHANGE);
  PCattachInterrupt(pin_E2B, isr2B, CHANGE);
  
  Serial.begin(9600);
}

int time = millis();

double speed = 0.0;

void loop() {
  m1.setSpeed(speed);
  m2.setSpeed(speed);
  
  if (millis() - time > 100) {
    time = millis();
    
    Serial.print(enc1.getCount());
    Serial.print("  ");
    Serial.print(enc2.getCount());
    Serial.print("\n");
  }
  
}

void isr1A() {
  enc1.encoderEvent(true);
}
void isr1B() {
  enc1.encoderEvent(false);
}

void isr2A() {
  enc2.encoderEvent(true);
}
void isr2B() {
  enc2.encoderEvent(false);
}

