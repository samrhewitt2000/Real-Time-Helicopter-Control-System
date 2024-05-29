#ifndef YAW_CONTROL_H_
#define YAW_CONTROL_H_

//*****************************************************************************
//
//      yaw_control.c
//
// What does this function do? (Replace)
//
//*****************************************************************************
//
// Author:          Caleb Westbury & Sam Hewitt
// Last modified:   May 2024
//
//*****************************************************************************
//
// Based on AUTHOR's FILENAME.c code from YEAR (replace bold if applicable otherwise delete)
//
//*****************************************************************************

#include <stdint.h>
#include "quad_enc.h"
#include "PWM.h"
#include "PID.h"
#include "driverlib/pin_map.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"

//extern volatile int32_t yaw_angle_int = 0;
//extern unsigned char get_yaw_ref_task;
extern int32_t current_alt_perc;

//*****************************************************************************
//
//*****************************************************************************
int32_t yaw_angle_ticks_to_decimal(int32_t quad_enc_ticks);



//*****************************************************************************
//
//*****************************************************************************
int32_t yaw_angle_ticks_to_int(int32_t quad_enc_ticks);


//*****************************************************************************************************
// converts yaw angle back into yaw encoder ticks - check with big sam or tutor
//*****************************************************************************************************
int32_t yaw_angle_to_ticks(int32_t angle);


//*****************************************************************************
//
//*****************************************************************************
void change_yaw_angle(int32_t yaw_angle_change, int32_t rotor_PWM);

//*****************************************************************************
//
//*****************************************************************************
void yaw_control_task(void);

//*****************************************************************************
//
//*****************************************************************************
void get_yaw_ref_task(void);


#endif /* YAW_CONTROL_H_ */
