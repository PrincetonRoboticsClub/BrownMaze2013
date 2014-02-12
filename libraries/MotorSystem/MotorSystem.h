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

#include <Arduino.h>
#include <Motor.h>

class MotorSystem{
  public:
    MotorSystem(Motor* left, Motor* right);
    ~MotorSystem();

    void arcade(double straight, double turn);
    void tank(double left, double right);

    void coast();
    void stop();

    double getLeft();
    double getRight();
	
	void test(double spd);
	
  private:
    Motor* mLeft, mRight;
	
};

#endif 
