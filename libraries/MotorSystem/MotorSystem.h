/**********************************************
 *                                            
 * Probotics - Princeton University           
 *                                            
 * MotorSystem: converts arcade drive and     
 *   tank drive values to motor pwm output  
 *
 * Compatible with the SN751440 H-Bridge
 *                                            
 **********************************************/


#ifndef MotorSystem_h
#define MotorSystem_h

//#include <Arduino.h>
#include <Motor.h>

class MotorSystem{
  public:
    MotorSystem(Motor* left, Motor* right);
    ~MotorSystem();

    void arcade(float straight, float turn);
    void tank(float left, float right);

    void coast();
    void brake();

    float getLeft();
    float getRight();
	
	void test(float spd);
	
  private:
    Motor* mLeft, *mRight;
	
};

#endif 
