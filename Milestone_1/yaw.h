/*
 * yaw.h
 *
 *  Created on: 1/05/2024
 *      Author: she108
 */

#ifndef YAW_H_
#define YAW_H_

typedef enum {

    PHASE_1,     // 00
    PHASE_2,     // 01
    PHASE_3,     // 11
    PHASE_4      // 10
} phase_t;

volatile static int32_t yaw_angle = 0;  // Global variable to store yaw angle

volatile static phase_t current_phase = PHASE_1;

volatile static phase_t prev_phase = PHASE_1;

phase_t get_current_phase(void);

void
PB_IntHandler(void);

void
initYaw (void);

#endif /* YAW_H_ */
