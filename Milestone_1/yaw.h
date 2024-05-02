/*
 * yaw.h
 *
 *  Created on: 1/05/2024
 *      Author: she108
 */

#ifndef YAW_H_
#define YAW_H_


/*

typedef enum {
	On,
	Off
} sensorState_t


// struct to hold each pin state for switch FSM

typedef struct {
	sensorState_t pinA;
	sensorState_t pinB;
} pinState_t;

sorry had to leave feel free to delete probs easier way
*/

void
PB0_IntHandler(void);

void
PB1_IntHandler(void);

void
initYaw (void);

#endif /* YAW_H_ */
