/**********************************************
 *                                            
 * Probotics - Princeton University           
 *                                            
 * class Motor: Low-level interface with a DC
 *	Motor connected to an H-Bridge 
 *
 * Compatible with the SN751440 H-Bridge
 *                                            
 **********************************************/

#ifndef	Motor_H
#define	Motor_H

#include <Arduino.h>
#define	MOTOR_MAX	255

typedef	int	pin_t;

 class Motor {
 	
 	public:

 		Motor(pin_t a1, pin_t a2, pin_t speedPinEN, bool invert);

 		// Instantly stops the motor at a BRAKE.
 		void	brake();

 		// Instantly stops the motor at a COAST.
 		void	coast();

 		void 	writeSpeed(float fSpeed);

 		float	getSpeed();



 	private:
 		pin_t	pA1, pA2;
 		pin_t	pEN;
 		bool	bInverted;

 		float	fCurrentSpeed;
 };

 #endif
