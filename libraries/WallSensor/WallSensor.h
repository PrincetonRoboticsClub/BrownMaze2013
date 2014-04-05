#ifndef WallSensor_h
#define WallSensor_h

typedef uint8_t pin_t

class WallSensor{
private:
   pin_t forwardPin, rightPin, leftPin;
public:
   WallSensor(pin_t fPin, pin_t rPin, pin_t lPin) {
      forwardPin = fPin;
      rightPin = rPin;
      leftPin = lPin;
   }

   uint8_t* getWalls(int direction) {
      uint8_t walls[4];

      walls[direction] = digitalRead(forwardPin);
      walls[(direction + 1)%4] = digitalRead(rightPin);
      walls[(direction + 2)%4] = 0;
      walls[(direction + 3)%4] = digitalRead(leftPin);

      return walls;
   }
};

#endif