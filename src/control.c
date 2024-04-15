/*
 * control.c
 *
 *  Created on: Aug 12, 2023
 *      Author: ADMIN
 */


#include "control.h"

extern PidObject c_roll,  c_pitch;


const PidConfig pid_roll  = { -1.5f, 0.01f, 0.01f };
const PidConfig pid_pitch = { 1.5f, 0.01f, 0.01f };
const float c_roll_imax  = 50;
const float c_pitch_imax = 50;

const float ctrl_dt = 40.0f / 1000.0f;

void CTRL_init(void) {

    // ½Ç¶ÈPID
    InitPidObject(&c_roll,  pid_roll,  c_roll_imax,  -c_roll_imax,  ctrl_dt);
    InitPidObject(&c_pitch, pid_pitch, c_pitch_imax, -c_pitch_imax, ctrl_dt);

}

void CTRL_update(void) {
    calcPid(&c_roll,  0, roll);
    calcPid(&c_pitch, 0, pitch);

//    calcPid(&cr_roll,  0, gx);
//    calcPid(&cr_pitch, 0, gy);

//    motor_roll_out  = c_roll.output;           // cr_roll.output
//    motor_pitch_out = c_pitch.output;          // cr_pitch.output

}

