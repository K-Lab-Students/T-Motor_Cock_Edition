/*
 * ak_drv.h
 *
 *  Created on: 24 нояб. 2023 г.
 *      Author: NakedSnake
 */

#ifndef INC_AK_DRV_H_
#define INC_AK_DRV_H_

#include <stdio.h>

typedef enum{
		CAN_PACKET_SET_DUTY = 0, //Duty cycle mode
		CAN_PACKET_SET_CURRENT, //Current loop mode
		CAN_PACKET_SET_CURRENT_BRAKE, // Current brake mode
		CAN_PACKET_SET_RPM, //Velocity mode
		CAN_PACKET_SET_POS, // Position mode
		CAN_PACKET_SET_ORIGIN_HERE, //Set origin mode
		CAN_PACKET_SET_POS_SPD, //Position velocity loop mode
} CAN_PACKET_ID;

typedef enum {
	COMM_FW_VERSION = 0, COMM_JUMP_TO_BOOTLOADER, COMM_ERASE_NEW_APP, COMM_WRITE_NEW_APP_DATA, COMM_GET_VALUES, //Get motor running parameters
	COMM_SET_DUTY, //Motor runs in duty cycle mode
	COMM_SET_CURRENT, //Motor runs in current loop mode
	COMM_SET_CURRENT_BRAKE, //Motor current brake mode operation
	COMM_SET_RPM, //Motor runs in current loop mode
	COMM_SET_POS, //Motor runs in position loop mode
	COMM_SET_HANDBRAKE, //Motor runs in handbrake current loop mode
	COMM_SET_DETECT, //Motor real-time feedback current position command
	COMM_ROTOR_POSITION=22,//Motor feedback current position
	COMM_GET_VALUES_SETUP=50,//Motor single or multiple parameter acquisitioninstructions
	COMM_SET_POS_SPD=91, // Motor runs in position velocity loop mode
	COMM_SET_POS_MULTI=92, // Set the motor movement to single-turn modeCOMM_SET_POS_SINGLE=93, // Set the motor motion to multi-turn mode, the range is
	COMM_SET_POS_UNLIMITED=94, //Save
	COMM_SET_POS_ORIGIN=95, //Set the motor origin
} COMM_PACKET_ID;

typedef struct {
	float position, speed, current;
	int temperature, error;
	uint8_t id, receiveId;
} MotorValues_t;

void motorInit(MotorValues_t motor, uint8_t id);

void comm_can_transmit_eid(uint32_t id, const uint8_t *data, uint8_t len);

void buffer_append_int32(uint8_t* buffer, int32_t number, int32_t *index);


void buffer_append_int16(uint8_t* buffer, int16_t number, int16_t *index);


void comm_can_set_pos(uint8_t controller_id, float pos);

void comm_can_set_pos_spd(uint8_t controller_id, float pos,int16_t spd, int16_t RPA );

void comm_can_set_origin(uint8_t controller_id, uint8_t set_origin_mode);

void comm_can_set_rpm(uint8_t controller_id, float rpm);

void comm_can_set_cb(uint8_t controller_id, float current);

void comm_can_set_current(uint8_t controller_id, float current);
void comm_can_set_duty(uint8_t controller_id, float duty);
void motor_receive(MotorValues_t* motor, uint8_t data[8], uint32_t frameId);
void motorCalibration(MotorValues_t* motor, float* maxLeft, float* maxRight);
#endif /* INC_AK_DRV_H_ */
