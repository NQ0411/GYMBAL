/*
 * PID.h
 *
 *  Created on: Aug 12, 2023
 *      Author: ADMIN
 */

#ifndef INC_PID_H_
#define INC_PID_H_


extern float motor_roll_out, motor_pitch_out;
extern float pitch,  roll; 		    // Å·À­½Ç

typedef struct{
	float exp;
	float Err;
	float PreErr;
	float Integral;
	float derivative;
	float Kp;
	float Ki;
	float Kd;
	float Pout;
	float Iout;
	float Dout;
	float output;
	float IntegralUpper;
	float IntegralLower;//IntegralUpper == IntegralLower means no limit;
	float dt;
} PidObject;

typedef struct{
	float Kp;
	float Ki;
	float Kd;
} PidConfig;

void  InitPidObject(PidObject *APidObfect,PidConfig APidConfig,float IntegralUpper,float integralLower,float dt);
void  calcPid(PidObject *APidObject,float exp,float data_now);
float DataLimit(float in,float Upper,float Lower);



#endif /* INC_PID_H_ */
